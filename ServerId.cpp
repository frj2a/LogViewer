// "$Date: $"
// "$Author: $"
// "$Revision: $"

#include "ServerId.h"

ServerId::ServerId(QString * endereco, quint16 * porta, QWidget *parent) : QDialog(parent), Ui::ServerIdClass() {
	setupUi(this);
	mEndereco = *endereco;
	mPorta.setNum(*porta);
	setModal(true);

	lineEditEndereco->setText(mEndereco);
//	QValidator * validator = new QRegExpValidator(QRegExp("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}"), this);
//	lineEditEndereco->setValidator(validator);

	lineEditPorta->setText(mPorta);
	lineEditPorta->setInputMethodHints(Qt::ImhDigitsOnly);
}

ServerId::~ServerId() {

}

void ServerId::accept() {
	mEndereco = lineEditEndereco->text();
	mPorta = lineEditPorta->text();
	emit endereco(mEndereco);
	emit porta(mPorta.toInt());
	setResult(QDialog::Accepted);
	close();
}
