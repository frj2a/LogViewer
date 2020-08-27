// "$Date: $"
// "$Author: $"
// "$Revision: $"

#include "LogViewer.h"
#include "ServerId.h"
#include <QtGui>
#include <QRect>
#include <QTimer>
#include <QString>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QNetworkInterface>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "LogPresenter.h"

#define	TIMER1				25	// ms
#define	TIMER2				TIMER1 * 3 / 5	// ms
#define	STANDARD_PORT		2010

LogViewer::LogViewer(QWidget *parent) : QMainWindow(parent), Ui::LogViewerClass() {
	setupUi(this);
	showMaximized();
	progressBar->setStyleSheet(QString (
		"	QProgressBar:horizontal {			"
		"		background: white;				"
		"		padding: 3px;					"
		"		text-align: right;				"
		"		margin-right: 8ex;				"
		"	}									"
		"	QProgressBar::chunk:horizontal {	"
		"		background: 					"
		"			qlineargradient(			"
		"				x1: 0, y1: 0.3, 		"
		"				x2: 1, y2: 0.3,			"
		"				stop: 0 green,			"
		"				stop: 1 white			"
		"			);							"
//		"		margin-right: 2px;				"
//		"		width: 20px;					"
		"	}									"
		) );

	progressBar->hide();
	label->hide();
	label_2->hide();
	labelAtual->hide();
	labelTotal->hide();
	pbPausa->hide();
	statusbar->addPermanentWidget(&mEstadoLeitura);
	statusbar->addPermanentWidget(&mMsgPermSB);
	mEstadoLeitura.setPixmap(QPixmap(":/imagens/livre"));
	mEstadoLeitura.setText("");
	mEstadoLeitura.hide();
	mMsgPermSB.setFont(QFont("Tahoma", 10, QFont::Black));
	mAplicativoVersao = QString(" - %1 v.: %2").arg(QString(APP_NAME)).arg(QString(APP_VERSION));
	mPresenter = new LogPresenter(textEdit);
	mNivelLog = mPresenter->normalLogLevel();
	spinBoxProf->setValue(mNivelLog);
	spinBoxProf->setMaximum(mPresenter->maxLogLevel());
	spinBoxProf->setMinimum(1);

	mTimer = new QTimer(this);
	mTimer->setSingleShot(false);
	mTimer->setInterval(TIMER1);

	QList<QHostAddress> listaEnderecos = QNetworkInterface::allAddresses();

	for (int i = listaEnderecos.size() - 1; i >= 0; --i) {
		if ( (listaEnderecos.at(i) != QHostAddress::LocalHost ) && ( listaEnderecos.at(i).toIPv4Address() ) ) {
			mEndRem = listaEnderecos.at(i).toString();
		}
	}

	// if we did not find one, use IPv4 localhost
	if (mEndRem.isEmpty())
		mEndRem = QHostAddress(QHostAddress::LocalHost).toString();

	connect(actionAbrir, SIGNAL(triggered()), this, SLOT(abrirArquivo()));
	connect(actionConectarServidor, SIGNAL(triggered()), this, SLOT(conectarServidor()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(mTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	connect(spinBoxProf, SIGNAL(valueChanged(int)), this, SLOT(mudarProf(int)));
	connect(pbClean, SIGNAL(clicked()), this, SLOT(limpar()));

	mModoOperacaoArquivo = true;

	// mEndRem = "192.168.0.101";
	mPorta = STANDARD_PORT;

	if (qApp->arguments().length() > 1) {
		if (	(qApp->arguments().at(1) != "/n")
			&&	(qApp->arguments().at(1) != "/N")
			&&	(qApp->arguments().at(1) != "-n")
			&&	(qApp->arguments().at(1) != "-N")
			&&	(qApp->arguments().at(1) != "/r")
			&&	(qApp->arguments().at(1) != "/R")
			&&	(qApp->arguments().at(1) != "-r")
			&&	(qApp->arguments().at(1) != "-R")
			) {

			mNomeArq = qApp->arguments().at(1);
			processarArquivo();
		} else {
			if (	(qApp->arguments().at(1) == "/n")
				||	(qApp->arguments().at(1) == "/N")
				||	(qApp->arguments().at(1) == "-n")
				||	(qApp->arguments().at(1) == "-N")
				||	(qApp->arguments().at(1) == "/r")
				||	(qApp->arguments().at(1) == "/R")
				||	(qApp->arguments().at(1) == "-r")
				||	(qApp->arguments().at(1) == "-R")
				) {
				mEndRem = qApp->arguments().at(2);
				int pos = mEndRem.indexOf(':');
				if (pos != -1) {
					mPorta = mEndRem.right(mEndRem.length() - pos - 1).toInt();
					mEndRem = mEndRem.left(pos);
				}
				/*
				 QMessageBox *msgbox = new QMessageBox(
				 QMessageBox::Information,
				 "Remote Address",
				 QString("Address: %1\nPort:    %2")
				 .arg(mEndRem)
				 .arg(QString::number(mPorta)),
				 QMessageBox::Ok,this);
				 msgbox->exec();
				 */
				mMsgPermSB.setText(QString(trUtf8("Remote Address: %1  port  %2")).arg(mEndRem).arg(QString::number(mPorta)) + mAplicativoVersao);
				statusbar->showMessage(trUtf8("Conectado"), 5000);
				mEstadoLeitura.show();

				mModoOperacaoArquivo = false;
				tcpSocket = new QTcpSocket(this);
				connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(lerLinhas()));

				mTimer->start();

				mudarProf(mNivelLog);

			} else {
				mMsgPermSB.setText(trUtf8("Waiting configuration") + mAplicativoVersao);
				statusbar->showMessage(trUtf8("Ready."), 5000);
			}
		}
	} else {
		mMsgPermSB.setText(trUtf8("Waiting configuration") + mAplicativoVersao);
		statusbar->showMessage(trUtf8("Ready."), 5000);
	}
}

LogViewer::~LogViewer() {
	if ( ! mModoOperacaoArquivo )	{
		desconectarServidor();
	}

	disconnect(spinBoxProf, SIGNAL(valueChanged(int)), this, SLOT(mudarProf(int)));
	disconnect(mTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	disconnect(actionConectarServidor, SIGNAL(triggered()), this, SLOT(conectarServidor()));
	disconnect(actionAbrir, SIGNAL(triggered()), this, SLOT(abrirArquivo()));

	delete mPresenter;
	delete mTimer;
}

void LogViewer::onTimer(void) {
//	blockSize = 0;
	tcpSocket->abort();
	tcpSocket->connectToHost(mEndRem, mPorta, QIODevice::ReadOnly);
}

void LogViewer::lerLinhas() {
	mEstadoLeitura.setPixmap(QPixmap(":/imagens/ocupado"));

	QDataStream entrada(tcpSocket);
	entrada.setVersion(QDataStream::Qt_4_8);
/*
	if (blockSize == 0) {
		if (tcpSocket->bytesAvailable() < (int) sizeof(quint16)) {
			return;
		}

//		entrada >> blockSize;
	}
*/
	if (tcpSocket->bytesAvailable() >= 36) {

		QString block;
		QString linha;
		while (!entrada.atEnd()) {
			entrada >> block;

			int pos = block.indexOf('\n');
			while ( pos > 0 )	{
				linha = block.mid( 0, pos );
				block = block.mid( pos+1 , block.size() - pos);
				mPresenter->acrescentarLinha(&linha);
				pos = block.indexOf('\n');
			}
		}
	}
	mTimer->start();
	QTimer::singleShot(TIMER2, this, SLOT(apagarSinal()));
}

void LogViewer::conectarServidor() {
	statusbar->showMessage(trUtf8("Waiting connection parameters."), 5000);

	ServerId * serverId = new ServerId(&mEndRem, &mPorta, this);

	connect(serverId, SIGNAL(endereco(QString&)), this, SLOT(mudarEnd(QString&)));
	connect(serverId, SIGNAL(porta(int)), this, SLOT(mudarPorta(int)));

	serverId->setModal(true);
	serverId->exec();

	/*
	 QMessageBox *msgbox = new QMessageBox(QMessageBox::Information,
	 "Conexão",
	 QString("Habilitando conexão em\n%1:%2")
	 .arg(mEndRem).arg(mPorta),
	 QMessageBox::Ok,this);
	 msgbox->exec();
	 */

	disconnect(serverId, SIGNAL(endereco(QString&)), this, SLOT(mudarEnd(QString&)));
	disconnect(serverId, SIGNAL(porta(int)), this, SLOT(mudarPorta(int)));

	delete serverId;

	mModoOperacaoArquivo = false;
	mMsgPermSB.setText(QString(trUtf8("Remote address: %1  port  %2")).arg(mEndRem).arg(QString::number(mPorta)) + mAplicativoVersao);

	tcpSocket = new QTcpSocket(this);
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(lerLinhas()));
	mTimer->start();
	mudarProf(mNivelLog);
	mEstadoLeitura.show();
}

void LogViewer::desconectarServidor() {
	mModoOperacaoArquivo = true;
	if (mTimer->isActive())	{
		mTimer->stop();
	}
	if (tcpSocket->isOpen())	{
		tcpSocket->abort();
	}
	disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(lerLinhas()));
	delete tcpSocket;
}

void LogViewer::abrirArquivo() {	// slot de menu
	mEstadoLeitura.hide();
	if (!mModoOperacaoArquivo) {
		desconectarServidor();
	}
	mNomeArq = QFileDialog::getOpenFileName(this, trUtf8("Open Log File"), "Z://DesenvEletronico", trUtf8("Log files (*.log *.txt)"));
	processarArquivo();
}

void LogViewer::processarArquivo() {
	QFile mArqLog;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	mArqLog.setFileName(mNomeArq);
	if (mArqLog.exists()) {
		mMsgPermSB.setText(QString(trUtf8("File %1")).arg(mNomeArq) + mAplicativoVersao);
		statusbar->showMessage(trUtf8("Processing ..."), 4000);
		if (mArqLog.open(QFile::ReadOnly | QFile::Text)) {
			qint64 tamanho = mArqLog.size();
			mPresenter->clear();
			QString linha;
			mudaProgresso(0,tamanho);
			QCoreApplication::processEvents();
			while (!mArqLog.atEnd()) {
				linha = codec->toUnicode(mArqLog.readLine(255));
				mPresenter->acrescentarLinha(&linha);
				qint64 posicao = mArqLog.pos();
				if ( ( posicao % 512 ) == 0) {
					mudaProgresso(posicao,tamanho);
					QCoreApplication::processEvents();
				}
			}
		}
	} else {
		if (mNomeArq.isEmpty()) {
			mNomeArq = trUtf8("(not specified)");
		}
		QMessageBox *msgbox = new QMessageBox(QMessageBox::Critical, "Arquivo não encontrado", mNomeArq, QMessageBox::Ok, this);
		msgbox->exec();
		mMsgPermSB.setText(trUtf8("Waiting configuration") + mAplicativoVersao);
		statusbar->showMessage(trUtf8("Ready."), 5000);
		delete msgbox;
	}
	mudaProgresso(0,0);
}

void LogViewer::mudarProf(int i) {
	mNivelLog = i;
	mPresenter->changeDepth(i);

}

void LogViewer::mudarEnd(QString& end) {
	mEndRem = end;
}

void LogViewer::mudarPorta(int porta) {
	mPorta = porta;
}

void LogViewer::apagarSinal(void) {
	mEstadoLeitura.setPixmap(QPixmap(":/imagens/livre"));
}

//!	\todo Melhorar a forma de indicar progresso
void LogViewer::mudaProgresso(qint64 pos, qint64 tot)	{
	if (tot == 0)	{
		progressBar->hide();
		label->hide();
		label_2->hide();
		labelAtual->hide();
		labelTotal->hide();
		pbPausa->hide();
	} else {
		if (! progressBar->isVisible())	{
			progressBar->show();
			label->show();
			label_2->show();
			labelAtual->show();
			labelTotal->show();
			pbPausa->show();
		}
		labelAtual->setText(QString("%1").arg(pos>>10));
		labelTotal->setText(QString("%1").arg(tot>>10));
		progressBar->setValue(int(pos*100/tot));

	}
}

void LogViewer::limpar()	{
	mPresenter->clear();
}
