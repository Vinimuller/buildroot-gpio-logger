## GPIO Logger

(Repositório: Vinimuller / buildroot-gpio-logger)

Esta aplicação realiza a leitura periódica do estado de determinadas GPIOs e grava esse estado em arquivo de log no sistema. O foco é oferecer uma solução leve para monitoramento de pinos digitais em sistemas embarcados com libgpiod, especialmente integrados via Buildroot.

🎯 Objetivo da aplicação

Ler o nível lógico de pinos GPIO configurados (por ex., 2, 3 ou outros) a cada segundo.
Gravar em arquivo de log com timestamp e nome amigável do pino.
Permitir associação de nomes personalizados aos pinos via arquivo de configuração.

🗂 Estrutura do repositório
```
buildroot-gpio-logger/
 ├── .gitignore
 └── package/
     └── gpio-logger/
         ├── Config.in               ← arquivo de integração Buildroot
         ├── gpio-logger.mk          ← arquivo make do pacote para Buildroot
         ├── S98gpio-logger          ← script de inicialização no boot
         └── files/
             ├── gpio-logger.conf    ← arquivo de configuração de nomes de pinos
         └── src/
             ├── main.c              ← código-fonte da aplicação
             ├── Makefile            ← Makefile da aplicação
```

Descrição dos principais arquivos

Config.in → Define a entrada do pacote no menuconfig do Buildroot, para habilitar o gpio-logger.

gpio-logger.mk → Instruções de build / instalação do binário para o sistema alvo via Buildroot.

main.c → Código-fonte da aplicação que lê os pinos e grava o log.

gpio-logger.conf → Permite mapear pinos para nomes amigáveis, ex:

```
2=StartBtn
3=StopBtn
17=Sensor1
```

S98gpio-logger → Script de inicialização colocado em /etc/init.d/ (ou equivalente) para rodar automaticamente no boot. O nome com prefixo “S98” garante ordenação no startup.