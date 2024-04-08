#!/bin/bash

VERSAO="1.0.0"
RPMCAMINHO=~/rpmbuild
FEDORA="fc39"

# Instalar programas necessários
echo "Verificando programas instalados..."
sudo dnf install g++ rpmdevtools rpmlint
echo " "
sleep 2

# Se pasta não existe, criar pasta
if [ ! -d "./bashsays-$VERSAO" ]; then
    echo "Criando pasta \"./bashsays-$VERSAO/\"..."
    mkdir "bashsays-$VERSAO/"
fi

# Compilar programa com g++
echo "Compilando programa para \"./bashsays-$VERSAO/bashsays\"..."
g++ -O3 "./src/bashsays.cpp" -o "./bashsays-$VERSAO/bashsays"
echo "Enviando manual em pt-BR para \"./bashsays-$VERSAO/bashsays-pt-BR.txt\"..."
cp "./src/bashsays-pt-BR.txt" "./bashsays-$VERSAO/bashsays-pt-BR.txt"
echo "Enviando manual em en-US para \"./bashsays-$VERSAO/bashsays-en-US.txt\"..."
cp "./src/bashsays-en-US.txt" "./bashsays-$VERSAO/bashsays-en-US.txt"

# Comprimir programa com tar
echo "Comprimindo programa em \"./bashsays-$VERSAO.tar.gz\"..."
tar -cf "bashsays-$VERSAO.tar.gz" "./bashsays-$VERSAO/"

# Criar rpmbuild se não existir
if [ ! -d $RPMCAMINHO ]; then
    cd ~
    rpmdev-setuptree
fi

# Copiar arquivos para rpmbuild
echo "Movendo \"./doc/bashsays.spec\" para \"$RPMCAMINHO/SPECS/\"..."
cp "doc/bashsays.spec" "$RPMCAMINHO/SPECS/"
echo "Movendo \"./bashsays-$VERSAO.tar.gz\" para \"$RPMCAMINHO/SOURCES/\"..."
cp "./bashsays-$VERSAO.tar.gz" "$RPMCAMINHO/SOURCES/"
cd $RPMCAMINHO

# Verificar bashsays.spec
echo " "
echo "Verificando \"$RPMCAMINHO/SPECS/bashsays.spec\"..."
rpmlint "./SPECS/bashsays.spec"
echo " "
sleep 2

# Criar build a partir de bashsays.spec
echo "Criando pacote rpm a partir de \"$RPMCAMINHO/SPECS/bashsays.spec\"..."
rpmbuild -ba "./SPECS/bashsays.spec"
tree

# Instalar programa no terminal (se desejar)
echo "Deseja instalar o bashsays no terminal? (y/n): "
read conf
if [[ $conf == "y" || $conf == "Y" ]]; then
    sudo dnf remove --assumeyes bashsays
    sudo dnf install --assumeyes "./RPMS/x86_64/bashsays-$VERSAO-1.$FEDORA.x86_64.rpm"
fi

# Exibir resultado final
echo "Prontin! =)"
echo " "
