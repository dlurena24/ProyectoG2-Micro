### Sistema de Puerta Automática Inteligente  
# Comunicación y Aplicación Web  

---

## 1. Arquitectura de la Aplicación Web

### Implementación
- HTML5 + CSS embebido directamente en firmware  
- JavaScript ligero con `fetch()` para enviar comandos  
- Página principal con:
  - Estado en tiempo real  
  - Botón "Abrir"  
  - Botón "Cerrar"  
  - Link a historial  
- Página de configuración `/config`  
- Página de logs con tabla dinámica

### Funcionamiento general
1. El usuario inicia sesión mediante Basic Auth.  
2. El navegador obtiene estado y logs mediante HTTP.  
3. El ESP32-S3 responde con páginas procesadas en RAM.  
4. Todas las operaciones quedan registradas en el log circular.

---

## 2. Interfaces de usuario
### Autenticación del usuario
- Antes de acceder a la app web, el usuario debe de autenticarse 
<img width="1920" height="1080" alt="4" src="https://github.com/user-attachments/assets/1c350821-324b-4571-9382-15d895d71c19" />

### Pantalla principal
- En esta pantalla es donde el usuario puede abrir o cerrar de forma manual la puerta desde la app web
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/54225694-bbed-4fc6-9e00-f2823c0a8da2" />

### Configuración de estado inicial de la puerta
- En esta interfaz el usuario configura el estado inicial en el que se encuentra la puerta
<img width="1920" height="1080" alt="2" src="https://github.com/user-attachments/assets/89afffaf-16f1-489a-a23c-2c0bc9ffb654" />

### Historial de actividad
- Aquí se puede observar los eventos registrados con su respectiva fecha y hora
<img width="1920" height="1080" alt="3" src="https://github.com/user-attachments/assets/c8ee6177-9c06-4f80-ac9a-2e4223af0295" />

---

## 3. Protocolo de Comunicación

La comunicación entre el usuario y el sistema se realiza mediante:

- **Wi-Fi STA**
- **Servidor HTTP embebido**
- Autenticación **Basic Auth**

Endpoints disponibles:

| Endpoint | Método | Función |
|----------|--------|----------|
| `/` | GET | Panel principal |
| `/door/open` | GET | Abrir puerta |
| `/door/close` | GET | Cerrar puerta |
| `/door/state` | GET | Estado actual |
| `/logs` | GET | Historial |
| `/config/set` | POST | Configuración inicial |

---

## 4. Funcionalidades 
### Monitoreo y Uso Manual de la Puerta
- El usuario puede abrir de forma manual la puerta de forma remota
- El usuario puede ver el estado actual de la puerta

### Registro de Actividad
- Se puede acceder a un registro de eventos donde el usuario puede obsaervar los eventos cuando se abrió y cerró la puerta
- Los eventos se registran con la fecha y la hora de cuando suceden

### Seguridad

- Basic Auth con contraseña codificada en Base64  
- Protección WPA2/WPA3 del router  

### Interacción Embebido con WebApp

Flujo típico:
Usuario presiona "Abrir"
- fetch('/door/open')
- ESP32-S3 activa GPIO del servo
- Registra evento
- Web recarga estado con '/door/state'




