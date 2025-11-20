#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <driver/gpio.h>
#include <esp_netif.h>
#include <esp_http_server.h>
#include <string.h>

#define WIFI_SSID      "K2300PT"
#define WIFI_PASS      "Chocolate-2307"
#define DOOR_PIN       1

static const char *TAG = "web-door";
static bool door_open = false;
static unsigned long last_action = 0;
//static const unsigned long debounce_ms = 200;

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
"<script>function updateEstado(){fetch('/door/state').then(res=>res.text()).then(t=>{document.getElementById('estado').textContent=t;});}"
"function activarPuerta(accion){fetch('/door/'+accion).then(()=>updateEstado());}"
"window.onload=updateEstado;</script></body></html>";

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

/* HTTP Server Handlers */
esp_err_t root_get_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html_form, HTTPD_RESP_USE_STRLEN);
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
    httpd_resp_sendstr(req, "<html><body><h2>Puerta abierta</h2><a href='/'>Volver</a></body></html>");
    return ESP_OK;
}

esp_err_t door_close_handler(httpd_req_t *req) {
    gpio_set_level(DOOR_PIN, 0); // LOW
    door_open = false;
    last_action = esp_log_timestamp();
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
        httpd_register_uri_handler(server, &root_uri);
        httpd_register_uri_handler(server, &state_uri);
        httpd_register_uri_handler(server, &open_uri);
        httpd_register_uri_handler(server, &close_uri);
    }
    return server;
}

void app_main(void) {
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

    // Espera a que WiFi esté conectado
    ESP_LOGI(TAG, "Esperando conexión WiFi...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    start_webserver();
    ESP_LOGI(TAG, "Servidor HTTP iniciado");
}
