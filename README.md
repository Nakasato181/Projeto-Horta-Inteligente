# 🌱 Projeto Horta Inteligente com ESP32
Este projeto implementa um sistema de monitoramento de umidade do solo para hortas utilizando um ESP32. Através de um sensor de umidade, um relé e uma interface web, é possível acompanhar a umidade do solo em tempo real e acionar automaticamente uma bomba de irrigação.

# 📋 Funcionalidades
Conexão Wi-Fi e IP fixo configurável.
Leitura da umidade do solo utilizando o pino analógico.
Interface web para monitoramento em tempo real.
Atualização periódica da hora via NTP (servidor de hora).
Acionamento automático de uma bomba de irrigação através de um relé.
Atualização da página web com a umidade e o horário atual.

# 🖥️ Interface Web
A interface exibe:

Umidade do solo em tempo real.
Hora atual sincronizada com servidor NTP.
Atualização automática a cada segundo.
Exemplo da tela:

|     HORTA INTELIGENTE     |
|---------------------------|
| 💧 Humidity: 45%          |
| Hora: 14:32:51            |

# 📡 Configuração da Rede Wi-Fi

Edite as seguintes constantes no código:

const char* ssid = "SEU_SSID";
const char* senha = "SUA_SENHA";

E configure o IP fixo:

IPAddress staticIP();  // IP fixo desejado

IPAddress gateway();   // Gateway do seu roteador

IPAddress subnet();    // Máscara de rede

# ⚙️ Componentes Utilizados
Componente	Descrição
ESP32	Microcontrolador principal
Sensor de Umidade	Sensor analógico conectado ao pino 34
Módulo Relé	Aciona a bomba de irrigação
Bomba de Água	Controlada pelo relé
Conexão Wi-Fi	Para exibir os dados via interface web
NTP	Sincronização de horário via internet

# 📦 Bibliotecas Utilizadas
Instale as seguintes bibliotecas na IDE Arduino:

ESPAsyncWebServer

NTPClient

# 📊 Lógica de Controle

O ESP32 lê o valor analógico do sensor de umidade.

Converte para uma porcentagem (0% = solo seco, 100% = solo muito úmido).

Se a umidade estiver abaixo de 55%, a bomba de água é ligada.

Se a umidade estiver acima de 55%, a bomba é desligada.

O valor da umidade e a hora atual são enviados para a página web.

A cada 10 segundos, o horário é atualizado via NTP.

# 📄 Estrutura do Código

setup()        // Inicializa Wi-Fi, servidor e NTP.
loop()         // Faz leitura da umidade, controla o relé e atualiza a hora.
processor()    // Insere valores dinâmicos na página HTML.

# 🕹️ Como Usar
Faça o upload do código para seu ESP32.
Acesse o IP configurado no navegador.
Monitore a umidade e a hora em tempo real.
O relé será acionado automaticamente quando necessário.

#🔗 Exemplo de Acesso

http://192.168.0.100  // Altere para o IP configurado

#📑 Licença
Este projeto é de livre uso e pode ser adaptado conforme suas necessidades.

📧 Contato  
Email: joo.nakasato@gmail.com
