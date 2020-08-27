#! /bin/bash
if [ -e Makefile ]
then
    make distclean -s && \
    rm -fR GeneratedFiles release debug moc_* qrc_* ui_* *.o 2> /dev/null
fi
QT_OTIMIZA="linux-g++"
MAQUINA=`uname -m`
if [ "$MAQUINA" = "x86_64" ]
then
        QT_OTIMIZA="linux-g++-64"
fi
echo -e - - -Compilando o programa '\033[1;33m'LogViewer'\033[0m' de recebimento de registros de depuração, para a plataforma '\033[1;33m'$MAQUINA'\033[0m' ...
`which qmake` -r -spec $QT_OTIMIZA "CONFIG-=debug_and_release release debug" "CONFIG+=release" 2> ~/lixo.txt && /usr/bin/make qmake_all
# lrelease TesteSerial.pro 2> /dev/null
make $MAKEOPTS -s && \
/usr/bin/strip -s LogViewer

# if [ "$MAQUINA" = "x86_64" ]
# then
#	/usr/bin/upx --best --ultra-brute --all-methods --all-filters --preserve-build-id LogViewer
# fi

mv LogViewer ~/bin/ && \
make $MAKEOPTS distclean -s && \
rm -fR GeneratedFiles debug release moc_* qrc_* ui_* *.o 2> /dev/null
if [ -e /usr/bin/cloc ]
then
    /usr/bin/cloc *.cpp *.h
fi
echo -e - - - Finalizando a compilação do programa '\033[1;33m'LogViewer'\033[0m' de recebimento de registros de depuração, para a plataforma '\033[1;33m'$MAQUINA'\033[0m' .
