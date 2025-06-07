# 🌿 Monitoramento Automático de Plantas

Este projeto é um sistema de monitoramento automático de plantas, desenvolvido para coletar e analisar dados ambientais, como temperatura, umidade e luminosidade, visando otimizar o cuidado com as plantas.

## 📦 Estrutura do Projeto

O repositório contém os seguintes diretórios e arquivos principais:

- `.devcontainer/` e `.vscode/`: Configurações de ambiente de desenvolvimento.
- `3rdparty/`: Dependências de terceiros utilizadas no projeto.
- `main.cpp`: Código-fonte principal do sistema.
- `CMakeLists.txt`: Script de build utilizando CMake.
- `compileWiringPi.sh`: Script para compilar a biblioteca WiringPi, utilizada para interação com hardware.
- `.gitmodules`: Configurações de submódulos Git.
- `.gitignore`: Arquivos e pastas ignorados pelo Git.

## 🚀 Como Executar

Para compilar e executar o projeto, siga os passos abaixo:

1. Clone o repositório:
   ```bash
   git clone https://github.com/ArtPires/Monitoramento-Automatico.git
   cd Monitoramento-Automatico
   ```

2. Compile a biblioteca WiringPi (necessária para interação com hardware):
   ```bash
   ./compileWiringPi.sh
   ```

3. Compile o projeto utilizando CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Execute o programa:
   ```bash
   ./Monitoramento-Automatico
   ```

## 🛠️ Dependências

Certifique-se de que as seguintes dependências estão instaladas no seu sistema:

- [WiringPi](http://wiringpi.com/): Biblioteca para controle de GPIO em Raspberry Pi.
- CMake: Ferramenta de automação de compilação.
- Compilador C++ (como g++).

## 🤝 Contribuição

Contribuições são bem-vindas! Sinta-se à vontade para abrir issues ou enviar pull requests.

## 📄 Licença

Este projeto está licenciado sob a [Licença MIT](LICENSE).
