WIRINGPI_PATH="/workspace/3rdparty/WiringPi"
LIB_WIRINGPI="libwiringpi.a"

if [ -z "$(ls -A "$WIRINGPI_PATH")" ]; then
    echo "O modulo [$WIRINGPI_PATH] nao esta completo!"
    echo "Atualize os modulos do projeto antes de continuar! [git submodule update --init --recursive]"
    exit 1
fi

cd /workspace/3rdparty/WiringPi/wiringPi

echo "Compilando $LIB_WIRINGPI..."
aarch64-linux-gnu-gcc -c -O2 *.c -I .


echo "Gerando arquivo $LIB_WIRINGPI"
ar rcs $LIB_WIRINGPI *.o

if [ -f "$LIB_WIRINGPI" ]; then
    echo "Arquivo [$LIB_WIRINGPI] disponivel no diretorio /workspace/3rdparty/pre-compiled-libs"
    mv $LIB_WIRINGPI /workspace/3rdparty/pre-compiled-libs/
else
    echo "Erro ao gerar o arquivo $LIB_WIRINGPI!"
fi

echo "Removendo arquivos de compilacao..."
rm *.o



