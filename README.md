#  Monitoramento Automático

Sistema embarcado com Raspberry Pi para monitoramento de umidade do solo e nível de água. Aciona automaticamente uma bomba d'água com base nas leituras dos sensores, ideal para sistemas de irrigação automatizados de baixo custo.

---

##  Estrutura do Projeto

O repositório contém os seguintes diretórios e arquivos principais:

- `.devcontainer/` e `.vscode/`: Configurações de ambiente de desenvolvimento.
- `3rdparty/`: Dependências de terceiros utilizadas no projeto.
- `main.cpp`: Código-fonte principal do sistema.
- `CMakeLists.txt`: Script de build utilizando CMake.
- `compileWiringPi.sh`: Script para compilar a biblioteca WiringPi, utilizada para interação com hardware.
- `.gitmodules`: Configurações de submódulos Git.
- `.gitignore`: Arquivos e pastas ignorados pelo Git.

---

##  Funcionalidades

- Leitura de umidade do solo com higrômetro via ADS1115 (conversor A/D)
- Verificação do nível de água com micro boia IP67
- Acionamento automático de bomba submersível via módulo relé
- Interface web para visualização em tempo real dos dados do sistema

---

##  Componentes Utilizados

- 1x **Raspberry Pi 3B (4GB RAM)**
- 1x **Sensor de umidade do solo (higrômetro)**
- 1x **Conversor analógico-digital ADS1115**
- 1x **Micro boia IP67 com cabo (sensor de nível de água)**
- 1x **Módulo relé de 1 canal (5V)**
- 1x **Mini bomba submersível para água**
- Jumpers, fios, fonte de alimentação 5V

---

##  Instalação e Execução

1. **Clone o repositório**
   ```bash
   git clone https://github.com/ArtPires/Monitoramento-Automatico.git
   cd Monitoramento-Automatico
   ```

2. **Instale as dependências**
   ```bash
   pip install -r requirements.txt
   ```

3. **Conecte os componentes conforme o esquema elétrico**
  
   Disponivel na documentação.

4. **Execute o sistema**
   ```bash
   python main.py
   ```


##  Como Funciona

- O sistema lê a umidade do solo e o nível da água em intervalos regulares.

- Se o solo estiver seco **e** o nível de água estiver suficiente:

  - A bomba submersível é acionada automaticamente.

- O usuário pode acompanhar os dados em tempo real pela interface web.

---

