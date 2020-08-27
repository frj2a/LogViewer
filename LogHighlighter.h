// "$Date: $"
// "$Author: $"
// "$Revision: $"

#ifndef LOGHIGHLIGHTER_H_
#define LOGHIGHLIGHTER_H_

/*
 * LogHighlighter.h
 *
 *  Created on: 12/01/2010
 *      Author: fares@gmail.com
 *
 *  Baseado no exemplo do Qt
 */

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

class QTextDocument;

class LogHighlighter: public QSyntaxHighlighter {
Q_OBJECT

public:
	//!	/brief Construtor, que recebe os tipos de entrada e a cor de seu realce.
	LogHighlighter(QStringList nomesTipos, QList<QColor> corTipos,
			QTextDocument *parent = 0);
	~LogHighlighter();

private:
	void highlightBlock(const QString &text);

private:
	QTextDocument 		*mParent;		// a ser removido no final da depuração
	struct HighlightingRule {
		QRegExp padrao;
		QTextCharFormat formato;
	};
	QVector<HighlightingRule>	highlightingRules;
	QTextCharFormat				formatoData;
	QTextCharFormat				formatoTipo;
	QTextCharFormat				formatoOrigem;
	QTextCharFormat				formatoSeparador;
	QTextCharFormat				formatoHexa;
	QTextCharFormat				formatoIndice;
	QTextCharFormat				formatoRegistro;
};

#endif /* LOGHIGHLIGHTER_H_ */
