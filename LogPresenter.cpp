// "$Date: $"
// "$Author: $"
// "$Revision: $"

#ifndef REV_MAJOR_CODE
#	define REV_MAJOR_CODE "0"
#endif

#ifndef REV_MINOR_CODE
#	define REV_MINOR_CODE "0"
#endif

#ifndef REV_PATCH_CODE
#	define REV_PATCH_CODE "1"
#endif

#include "LogPresenter.h"
#include <QtGui>
#include <QString>
#include <QList>
#include <QColor>
#include <QTextEdit>
#include "LogHighlighter.h"

#define TAMANHO_FONTE		8

LogPresenter::LogPresenter(QTextEdit *TEparent, QStringList *TiposErro, QList<QColor> *CorTiposErros) : QObject() {
	mTextEditFromParent = TEparent;
	mNomesTipos.clear();
	if (TiposErro == NULL ) {
		mNomesTipos
		// nível 1
				<< "Emergencia"
		// nível 2
				<< "ErroCritico"
		// nível 3
				<< "Erro"
		// nível 4
				<< "Alerta"
		// nível 5
				<< "Advertencia"
		// nível 6
				<< "Observacao"
		// nível 7
				<< "Informacao"
		// nível 8
				<< "Depuracao"
		// fim da lista
		;
	} else {
		mNomesTipos = *TiposErro;
	}
	mCorTipos.clear();
	if (CorTiposErros == NULL) {
		mCorTipos
		//	nível 1
				<< QColor(127, 0, 127)
		//	nível 2
				<< QColor(127, 0, 0)
		//	nível 3
				<< QColor(191, 63, 0)
		//	nível 4
				<< QColor(191, 127, 0)
		//	nível 5
				<< QColor(191, 191, 0)
		//	nível 6
				<< QColor(63, 127, 63)
		//	nível 7
				<< QColor(127, 191, 127)
		//	nível 8
				<< QColor(159, 191, 159)
		// fim da lista
		;
	} else {
		for (int i = 0; i < CorTiposErros->count() ; i++) {
			mCorTipos << CorTiposErros->at(i);
		}
	}

	// os três níveis mais baixos são passíveis de serem ignorados em regime.
	mNivelLog = mNomesTipos.count() - 2;
	mMaxLargTipo = mMaxLargOrigem = 0;
	mLogHighlighter = new LogHighlighter(mNomesTipos, mCorTipos, mTextEditFromParent->document());
	mTextoArmazem.clear();
	changeDepth(mNivelLog);

	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
	font.setPointSize(TAMANHO_FONTE);
	mTextEditFromParent->setFont(font);
}

LogPresenter::~LogPresenter() {
	delete mLogHighlighter;
}

qint16 LogPresenter::maxLogLevel() {
	return mNomesTipos.count();
}

qint16 LogPresenter::normalLogLevel() {
	return mNomesTipos.count();
}

void LogPresenter::acrescentarLinha(QString *linha) {
	*linha = linha->trimmed();
	if (!linha->isEmpty()) {
		QRegExp provRegExp("TP\\:([A-Za-z_0-9]*\\b)");
		int index;
		int length;
		if ((index = provRegExp.indexIn(*linha)) >= 0) {
			if ((length = provRegExp.matchedLength()) > mMaxLargTipo) {
				mMaxLargTipo = length + 1;
			}
			linha->insert(index + length, QString(mMaxLargTipo - length, ' '));
		}
		provRegExp.setPattern("(OR\\:[\\:A-Za-z_0-9\\[\\]]*\\b\\]*)");
		if ((index = provRegExp.indexIn(*linha)) >= 0) {
			if ((length = provRegExp.matchedLength()) > mMaxLargOrigem) {
				mMaxLargOrigem = length + 1;
			}
			linha->insert(index + length, QString(mMaxLargOrigem - length, ' '));
		}
		mTextoArmazem.append(*linha)	; 	//	(linha.append('\n')*/);
		if (mRegExpVal.indexIn(*linha) != -1)	{
			mTextEditFromParent->append(*linha);
			mTextEditFromParent->moveCursor(QTextCursor::End);
		}
	}
}

void LogPresenter::changeDepth(int i) {
	mNivelLog = i;
	QString texto;
	QString padrao;
	QString regexp;
	regexp = "\\b(";

	padrao = mNomesTipos.at(0);
	regexp.append(padrao);
	for (int i = 1; i < mNivelLog; i++) {
		padrao = mNomesTipos.at(i);
		regexp.append("|").append(padrao);
	}
	regexp.append(")");
	mRegExpVal.setPattern(regexp);

	texto.clear();

	foreach (QString linha, mTextoArmazem) {
		if (mRegExpVal.indexIn(linha) != -1) {
			texto.append(linha).append(QChar::ParagraphSeparator);
		}
	}

	mTextEditFromParent->setPlainText(texto.trimmed());
	mTextEditFromParent->moveCursor(QTextCursor::End);
}

void LogPresenter::clear(void)	{
	mTextoArmazem.clear();
	mTextEditFromParent->clear();
}

void LogPresenter::Log(int Tip, QString Cls, QString Msg, QString Utf8Msg)	{	// slot
	QString entrada;
	entrada.append(QDateTime::currentDateTime().toString("yyyy-MMM-dd hh:mm:ss.zzz"));
	entrada.append(" TP:");
	entrada.append(mNomesTipos.at(Tip - 1));	// "Tip" começa em 1.
	entrada.append(" OR:");
	entrada.append(Cls);
	entrada.append(" RG:");
	entrada.append(QString::fromUtf8(Msg.toStdString().c_str()).append(Utf8Msg));
	entrada.append("\n");
	acrescentarLinha(&entrada);
}
