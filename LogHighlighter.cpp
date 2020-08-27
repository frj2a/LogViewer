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

//
// LogHighlighter.cpp
//
//  Created on: 12/01/2010
//      Author: FAres
//
//  Baseado no exemplo do Qt
//

#include <QtGui>
#include "LogHighlighter.h"

//!	No construtor se formatam as regras de destaque de sintaxe.
//!	É importante que receba o "QTextDocument" onde vai inserir os dados,
//!	pois, caso contrário, se forneça apenas um "QObject" como "parent",
//!	que é uma das opções de construção do "QSyntaxHighlighter", a informação
//!	de realce é perdida.
LogHighlighter::LogHighlighter(QStringList nomesTipos, QList<QColor> corTipos,
		QTextDocument *parent) :
	QSyntaxHighlighter(parent) {

	HighlightingRule regra;

	// data -------------------------------------------------------------------
	formatoData.setForeground(Qt::darkGray);
	formatoData.setFontItalic(true);
	//	formatoData.setFontWeight(QFont::Bold);
	regra.padrao = QRegExp("[\\d]{4,4}\\-[\\w]{3,3}\\-[\\d]{2,2}\\ [\\d]{2,2}\\:[\\d]{2,2}\\:[\\d]{2,2}\\.[\\d]{3,3}");
	//	[0-9]{4,4}\-[A-Za-z]{3,3}\-[0-9]{2,2}\ [0-9]{2,2}\:[0-9]{2,2}\:[0-9]{2,2}
	regra.formato = formatoData;
	highlightingRules.append(regra);


	// tipo de entrada --------------------------------------------------------
	formatoTipo.setFontWeight(QFont::Bold);

	QString padrao;
	QColor cor;
	for (int i = 0; i < nomesTipos.length(); i++) {
		padrao = nomesTipos.at(i);
		cor = corTipos.at(i);
		formatoTipo.setForeground(QBrush(cor));
		regra.padrao = QRegExp(QString("(TP:").append(padrao).append("\\b)"));
		regra.formato = formatoTipo;
		highlightingRules.append(regra);
	}

	// originador -------------------------------------------------------------
	formatoOrigem.setForeground(Qt::darkGreen);
	formatoOrigem.setFontItalic(true);
	//	regra.padrao = QRegExp("(\\ \\-\\-\\-\\ [\\[\\]\w_\d]*\\ \\-\\-\\-\\ )");
	regra.padrao = QRegExp("OR\\:[\\:\\[\\w\\d]*\\b\\]*");
	regra.formato = formatoOrigem;
	highlightingRules.append(regra);

	// mensagem da entrada no registro ----------------------------------------
	formatoRegistro.setForeground(QColor(63,63,63));
	formatoRegistro.setFontItalic(true);
	//	regra.padrao = QRegExp("(\\ \\-\\-\\-\\ [\\[\\]\w_\d]*\\ \\-\\-\\-\\ )");
	regra.padrao = QRegExp("RG\\:[\\[\\:\\'\\-\\/\\.\\(\\)\\,\\º\\°\\ \\w_\\d\\]]*\\]*\\b\\]*");
	regra.formato = formatoRegistro;
	highlightingRules.append(regra);

	// EXTRA - destaca valores hexadecimais dentro da mensagem da entrada -----
	formatoHexa.setForeground(Qt::darkBlue);
	formatoHexa.setFontWeight(QFont::Bold);
	formatoHexa.setFontItalic(true);
	regra.formato = formatoHexa;
	regra.padrao = QRegExp("\\=\\ ([A-Fa-f\\d]{2,2}\\ ?){1,12}");
	highlightingRules.append(regra);

	// EXTRA - destaca índices ------------------------------------------------
	formatoIndice.setForeground(Qt::darkRed);
	formatoIndice.setFontWeight(QFont::Bold);
	formatoIndice.setFontItalic(true);
	regra.formato = formatoIndice;
	regra.padrao = QRegExp("\\[\\d*\\]");
	highlightingRules.append(regra);

	// separadores ------------------------------------------------------------
	formatoSeparador.setForeground(QBrush(QColor(239, 239, 239)));
	regra.formato = formatoSeparador;
	regra.padrao = QRegExp("(\\-{1,3})");
	highlightingRules.append(regra);
	regra.padrao = QRegExp("(TP:|OR:|RG:)");
	highlightingRules.append(regra);
	//	regra.padrao = QRegExp("OR:");
	//	highlightingRules.append(regra);
	//	regra.padrao = QRegExp("RG:");
	//	highlightingRules.append(regra);
}

LogHighlighter::~LogHighlighter()	{
	mParent = NULL;
}

void LogHighlighter::highlightBlock(const QString &texto) {
	foreach (const HighlightingRule &regra, highlightingRules) {
			QRegExp expressao(regra.padrao);
			int indice = expressao.indexIn(texto);
			while (indice >= 0) {
				int comprim = expressao.matchedLength();
				setFormat(indice, comprim, regra.formato);
				indice = expressao.indexIn(texto, indice + comprim);
			}
		}
/*
	setCurrentBlockState(0);

	int indiceInicio = 0;
	if (previousBlockState() != 1)	{
		indiceInicio = commentStartExpression.indexIn(text);
	}

	while (indiceInicio >= 0) {

		int indiceFim = commentEndExpression.indexIn(text, indiceInicio);
		int commentLength;
		if (indiceFim == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - indiceInicio;
		} else {
			commentLength = indiceFim - indiceInicio
					+ commentEndExpression.matchedLength();
		}
		setFormat(indiceInicio, commentLength, multiLineCommentFormat);
		indiceInicio = commentStartExpression.indexIn(text, indiceInicio + commentLength);
	}
*/

}

//	RegExp para separação por capturas:
// (\d{4,4}\-[\w]{3,3}\-\d{2,2}\ \d{2,2}\:\d{2,2}\:\d{2,2})\ (Emergencia|Alerta|ErroCritico|Erro|Advertencia|Observacao|Informacao|Depuracao)\ ([\w_\d]*)\ ([\w_\d\(\)\º\.\0133\0135\ \:\=]*$)
//

//
// data:
// (\d{4,4}\-[\w]{3,3}\-\d{2,2}\ \d{2,2}\:\d{2,2}\:\d{2,2})
//

//
// primeira expressão
// [\w]{5,}\b
// \bEmergencia|Alerta|ErroCritico|Erro|Advertencia|Observacao|Informacao|Depuracao\b
//
