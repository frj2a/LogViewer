// "$Date: $"
// "$Author: $"
// "$Revision: $"

#ifndef LOGPRESENTER_H
#define LOGPRESENTER_H

#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QColor>
#include <QList>

//class QString;
class QTextEdit;
class LogHighlighter;

class LogPresenter : public QObject
{
	Q_OBJECT

public:
	LogPresenter(QTextEdit *TEparent, QStringList *TiposErro = NULL,
							QList<QColor> *CorTiposErros = NULL);
	~LogPresenter();

	//!	\brief
	qint16			maxLogLevel(void);

	//!	\brief
	qint16			normalLogLevel(void);

	//!	\brief Formatação da linha, acrescentando espaços para tabular.
	void			acrescentarLinha(QString *linha);

	//!	\brief	Limpa acumulador de linhas de registro para nova sessão.
	void			clear(void);

private:

	//!	\brief
	QTextEdit		*mTextEditFromParent;

	//!	\brief
	QStringList		mTextoArmazem;

	//!	\brief
	QStringList		mNomesTipos;

	//!	\brief
	QList<QColor>	mCorTipos;

	//!	\brief
	QList<QRegExp>	mRegExpTipos;

	//!	\brief
	LogHighlighter  *mLogHighlighter;

	//!	\brief
	QRegExp			mRegExpVal;

	//!	\brief
	int				mMaxLargOrigem;

	//!	/brief
	int				mMaxLargTipo;

public:

	//!	/brief
	qint16			mNivelLog;

public
slots:

	//!	/brief
	void			Log(int, QString, QString, QString Utf8Msg = "");

	//!	\brief Altera qual texto será exibido em função do nível de detalhe solicitado.
	void			changeDepth(int i);
};

#endif // LOGPRESENTER_H
