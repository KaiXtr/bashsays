$VERSAO = "1.0.0"
$RPMCAMINHO = "~/rpmbuild/"

# Se pasta não existe, criar pasta
cd ..
if [! -d "./bashsays-$VERSAO"]; then
    echo "Criando pasta \"./bashsays-$VERSAO/\"..."
    mkdir "bashsays-$VERSAO/"
fi

# Compilar programa com g++
echo "Compilando programa para \"./bashsays-$VERSAO/bashsays\"..."
g++ -O3 "./src/bashsays.cpp" -o "./bashsays-$VERSAO/bashsays"

# Comprimir programa com tar
echo "Comprimindo programa em \"./bashsays-$VERSAO.tar.gz\"..."
tar -cf "bashsays-$VERSAO.tar.gz" "./bashsays-$VERSAO/"

# Copiar arquivos para rpmbuild
echo "Movendo \"./doc/bashsays.spec\" para \"$RPMCAMINHO/SPECS/\"..."
cp "doc/bashsays.spec" "$RPMCAMINHO/SPECS/"
echo "Movendo \"./bashsays-$VERSAO.tar.gz\" para \"$RPMCAMINHO/SOURCES/\"..."
cp "./bashsays-$VERSAO.tar.gz" "$RPMCAMINHO/SOURCES/"
cd $RPMCAMINHO

# Instalar programas necessários
sudo dnf install rpmdevtools rpmlint

# Verificar bashsays.spec
echo "Verificando \"$RPMCAMINHO/SPECS/bashsays.spec\"..."
rpmlint "./SPECS/bashsays.spec"

# Criar build a partir de bashsays.spec
echo "Criando pacote rpm a partir de \"$RPMCAMINHO/SPECS/bashsays.spec\"..."
rpmbuild -ba "./SPECS/bashsays.spec"

# Exibir resultado final
tree