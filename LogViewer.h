// "$Date: $"
// "$Author: $"
// "$Revision: $"

#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QFile>
#include "ui_LogViewer.h"

class QTcpSocket;
class LogPresenter;

class LogViewer: public QMainWindow, public Ui::LogViewerClass {
Q_OBJECT

public:
	LogViewer(QWidget *parent = 0);
	~LogViewer();
	void processarArquivo();
	void mudaProgresso(qint64 pos, qint64 tot);

private:
	QString		 mAplicativoVersao;
	QTimer		 *mTimer;
	QString		 mNomeArq;
	QFile		 mArqLog;
	bool		 mModoOperacaoArquivo;
	QString		 mEndRem;
	quint16		 mPorta;
	QTcpSocket	 *tcpSocket;
	quint16		 blockSize;
	QLabel		 mMsgPermSB;
	QLabel		 mEstadoLeitura;
	QString		 mTexto;
	LogPresenter *mPresenter;
	int			 mNivelLog;

private
	slots:
	void abrirArquivo();
	void conectarServidor();
	void desconectarServidor();
	void onTimer();
	void lerLinhas();
	void mudarEnd(QString& end);
	void mudarPorta(int porta);
	void apagarSinal();
	void mudarProf(int i);
	void limpar();
};

#endif // LOGVIEWER_H
