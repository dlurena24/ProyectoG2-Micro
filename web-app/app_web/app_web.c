#include "app_web.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <driver/gpio.h>
#include <esp_netif.h>
#include <esp_http_server.h>
#include <string.h>
#include <sys/param.h>
#include <time.h>
#include "mbedtls/base64.h"

#define WIFI_SSID      "K2300PT"
#define WIFI_PASS      "Chocolate-2307"
#define HTTP_AUTH_USERNAME "admin"
#define HTTP_AUTH_PASSWORD "akmc23"
#define LOG_CAPACITY 25
#define DOOR_PIN       18

static const char *TAG = "web-door";
static bool door_open = false;
static unsigned long last_action = 0;

typedef struct {
    time_t timestamp;
    bool door_state;
} log_entry_t;

static log_entry_t logs[LOG_CAPACITY] = {0};
static int log_count = 0;

static char html[4000];


static const char *html_form =
"<!DOCTYPE html><html><head>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>Puerta ESP32</title><style>"
"body{font-family:Arial,sans-serif;display:flex;flex-direction:column;align-items:center;justify-content:center;min-height:100vh;margin:0;background:#f5f5f5;}"
".panel{background:#fff;padding:20px;border-radius:12px;box-shadow:0 2px 6px rgba(0,0,0,.15);text-align:center;}"
"button{font-size:1.1em;padding:10px 20px;margin:8px;border-radius:8px;border:1px solid #0b7;background:#e6f7e6;}"
"</style></head><body><div class=\"panel\"><h2>Estado de la puerta</h2>"
"<div id=\"estado\">Desconocido</div><button onclick=\"activarPuerta('open')\">Abrir Puerta</button>"
"<button onclick=\"activarPuerta('close')\">Cerrar Puerta</button></div>"
"<a href=\"/logs\"><button class='btn'>Ver historial</button></a>"
"<script>function updateEstado(){fetch('/door/state').then(res=>res.text()).then(t=>{document.getElementById('estado').textContent=t;});}"
"function activarPuerta(accion){fetch('/door/'+accion).then(()=>updateEstado());}"
"window.onload=updateEstado;</script></body></html>";

const char *htmlConfigPage =
"<!DOCTYPE html><html><head>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>Configurar puerta</title>"
"<style>"
"* { box-sizing: border-box; }"
"body { font-family: Arial, sans-serif; margin:0; padding:0; background:#f0f2f5; }"
".container { max-width: 600px; margin: 0 auto; padding: 16px; }"
".card { background:#fff; padding: 16px; border-radius: 12px; box-shadow: 0 2px 8px rgba(0,0,0,.1); }"
"h2 { margin-top: 0; }"
"label { display:block; margin:12px 0 6px; font-weight:600; }"
".row { display:flex; gap:12px; align-items:center; justify-content:flex-start; flex-wrap:wrap; }"
".btn { padding:10px 16px; border-radius:8px; border:1px solid #0b7; background:#e0f2f1; cursor:pointer; }"
"@media (max-width: 600px) { .container { padding: 8px; } }"
"</style></head><body>"
"<div class=\"container\"><div class=\"card\">"
"<h2>Configurar estado inicial de la puerta</h2>"
"<form id=\"configForm\" action=\"/config/set\" method=\"POST\">"
"<label>Estado inicial</label>"
"<div class=\"row\">"
"<input type=\"radio\" id=\"open\" name=\"state\" value=\"OPEN\" checked>"
"<label for=\"open\">Abierta</label>"
"<input type=\"radio\" id=\"closed\" name=\"state\" value=\"CLOSED\" style=\"margin-left:16px;\">"
"<label for=\"closed\">Cerrada</label>"
"</div>"
"<div style=\"margin-top:12px;\"><button class=\"btn\" type=\"submit\">Aplicar</button></div>"
"</form>"
"<div id=\"status\" style=\"margin-top:8px; color:#555; font-size:0.9em;\"></div>"
"</div></div>"
"<script>"
"document.getElementById('configForm').addEventListener('submit', function(e) {"
"e.preventDefault();"
"const formData = new FormData(this);"
"const data = new URLSearchParams(formData);"
"fetch('/config/set', { method: 'POST', body: data })"
".then(res => res.text())"
".then(txt => { document.getElementById('status').innerText = txt; });"
"});"
"</script>"
"</body></html>";


/* WiFi event handler */
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                              int32_t event_id, void* event_data) {
    if (event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "retry to connect to the AP");
    }
}

/*Handler log event*/
static void add_log_event(bool is_open) {
    time_t now;
    time(&now);
    if (log_count < LOG_CAPACITY) {
        logs[log_count].timestamp = now;
        logs[log_count].door_state = is_open;
        log_count++;
    } else {
        // Desplaza todo a la izquierda (FIFO)
        memmove(logs, logs+1, (LOG_CAPACITY-1)*sizeof(log_entry_t));
        logs[LOG_CAPACITY-1].timestamp = now;
        logs[LOG_CAPACITY-1].door_state = is_open;
    }
}

/* Connect to WiFi station */
void wifi_init_sta(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);

    wifi_config_t wifi_config = { 0 };
    strcpy((char*)wifi_config.sta.ssid, WIFI_SSID);
    strcpy((char*)wifi_config.sta.password, WIFI_PASS);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

//Funcion para verificar credenciales
static bool check_http_basic_auth(httpd_req_t *req) {
    char auth_hdr[128] = {0};
    if (httpd_req_get_hdr_value_str(req, "Authorization", auth_hdr, sizeof(auth_hdr)) == ESP_OK) {
        // Esperado: "Basic base64(user:pass)"
        char expected[64];
        snprintf(expected, sizeof(expected), "%s:%s", HTTP_AUTH_USERNAME, HTTP_AUTH_PASSWORD);

        // Extrae el base64
        char* base64_start = strstr(auth_hdr, "Basic ");
        if (base64_start) {
            base64_start += 6; // salta "Basic "
            unsigned char decoded[64] = {0};
            size_t decoded_len = 0;
            mbedtls_base64_decode(decoded, sizeof(decoded), &decoded_len, (const unsigned char*)base64_start, strlen(base64_start));
            decoded[decoded_len] = 0;
            if (strcmp((const char*)decoded, expected) == 0) {
                return true; // Autenticación OK
            }
        }
    }
    // Si llega aquí: no autenticado
    httpd_resp_set_status(req, "401 Unauthorized");
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "WWW-Authenticate", "Basic realm=\"ESP32 Login\"");
    httpd_resp_send(req, "Autenticación requerida", HTTPD_RESP_USE_STRLEN);
    return false;
}

/* HTTP Server Handlers */
esp_err_t root_get_handler(httpd_req_t *req) {
    if (!check_http_basic_auth(req)) return ESP_FAIL;
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html_form, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t logs_get_handler(httpd_req_t *req) {
    struct tm timeinfo;
    int idx = 0;

    idx += snprintf(html + idx, sizeof(html) - idx,
        "<!DOCTYPE html><html><head>"
        "<title>Historial de puerta</title>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
        "<style>body{font-family:Arial,sans-serif;}table{border-collapse:collapse;width:100%%;}th,td{border:1px solid #ccc;padding:8px;text-align:left;}tr:nth-child(even){background:#f3f3f3;}</style>"
        "</head><body><h2>Historial puerta</h2><table><tr><th>Fecha/Hora</th><th>Estado</th></tr>"
    );
    for (int i = 0; i < log_count; i++) {
        localtime_r(&logs[i].timestamp, &timeinfo);
        if (idx + 128 < sizeof(html)) { // Espacio suficiente para una línea
            idx += snprintf(
                html + idx, sizeof(html) - idx,
                "<tr><td>%02d:%02d - %d/%d/%d</td><td>%s</td></tr>",
                timeinfo.tm_hour, timeinfo.tm_min,
                timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900,
                logs[i].door_state ? "ABIERTO" : "CERRADO"
            );
        } else {
            idx += snprintf(html + idx, sizeof(html) - idx,
                "<tr><td colspan=2>... (más registros no mostrados)</td></tr>");
            break;
        }
    }
    idx += snprintf(html + idx, sizeof(html) - idx, "</table><a href=\"/\"><button class='btn'>Volver</button></a></body></html>");

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html, idx);
    return ESP_OK;
}


esp_err_t config_get_handler(httpd_req_t *req) {
    if (!check_http_basic_auth(req)) return ESP_FAIL;
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, htmlConfigPage, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t config_set_handler(httpd_req_t *req) {
    char buf[256];
    int total_len = req->content_len;
    int ret, remaining = total_len, idx = 0;

    // Lee el cuerpo (POST data)
    while (remaining > 0) {
        if ((ret = httpd_req_recv(req, buf + idx, MIN(remaining, (sizeof(buf)-1) - idx))) <= 0) {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        remaining -= ret;
        idx += ret;
    }
    buf[idx] = '\0';

    // Busca el parámetro state (formato: state=OPEN o state=CLOSED)
    char *state_ptr = strstr(buf, "state=");
    if (state_ptr) {
        char *value = state_ptr + 6;
        if (strncmp(value, "OPEN", 4) == 0) {
            gpio_set_level(DOOR_PIN, 1);
            door_open = true;
            httpd_resp_sendstr(req, "Estado inicial aplicado: OPEN");
        } else if (strncmp(value, "CLOSED", 6) == 0) {
            gpio_set_level(DOOR_PIN, 0);
            door_open = false;
            httpd_resp_sendstr(req, "Estado inicial aplicado: CLOSED");
        } else {
            httpd_resp_sendstr(req, "Valor de estado no válido");
        }
    } else {
        httpd_resp_set_status(req, "400 Bad Request");
        httpd_resp_sendstr(req, "Faltan datos");
    }
    return ESP_OK;
}


esp_err_t door_state_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/plain");
    httpd_resp_send(req, door_open ? "OPEN" : "CLOSED", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t door_open_handler(httpd_req_t *req) {
    gpio_set_level(DOOR_PIN, 1); // HIGH
    door_open = true;
    last_action = esp_log_timestamp();
    add_log_event(true);
    httpd_resp_sendstr(req, "<html><body><h2>Puerta abierta</h2><a href='/'>Volver</a></body></html>");
    return ESP_OK;
}

esp_err_t door_close_handler(httpd_req_t *req) {
    gpio_set_level(DOOR_PIN, 0); // LOW
    door_open = false;
    last_action = esp_log_timestamp();
    add_log_event(false);
    httpd_resp_sendstr(req, "<html><body><h2>Puerta cerrada</h2><a href='/'>Volver</a></body></html>");
    return ESP_OK;
}

httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t root_uri = { .uri = "/", .method = HTTP_GET, .handler = root_get_handler };
        httpd_uri_t state_uri = { .uri = "/door/state", .method = HTTP_GET, .handler = door_state_handler };
        httpd_uri_t open_uri = { .uri = "/door/open", .method = HTTP_GET, .handler = door_open_handler };
        httpd_uri_t close_uri = { .uri = "/door/close", .method = HTTP_GET, .handler = door_close_handler };
        httpd_uri_t config_uri = { .uri = "/config", .method = HTTP_GET, .handler = config_get_handler };
        httpd_uri_t config_set_uri = {.uri = "/config/set", .method = HTTP_POST, .handler = config_set_handler};
        httpd_uri_t logs_uri = { .uri = "/logs", .method = HTTP_GET, .handler = logs_get_handler };
        httpd_register_uri_handler(server, &logs_uri);
        httpd_register_uri_handler(server, &config_set_uri);
        httpd_register_uri_handler(server, &config_uri);
        httpd_register_uri_handler(server, &root_uri);
        httpd_register_uri_handler(server, &state_uri);
        httpd_register_uri_handler(server, &open_uri);
        httpd_register_uri_handler(server, &close_uri);
    }
    return server;
}

void app_web_init(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << DOOR_PIN,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    wifi_init_sta();

    ESP_LOGI(TAG, "Esperando conexión WiFi...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    start_webserver();
    ESP_LOGI(TAG, "Servidor HTTP iniciado");
    // NO bucle infinito aquí; el main() será quien gestione el ciclo de la app
}
