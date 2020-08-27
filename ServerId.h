// "$Date: $"
// "$Author: $"
// "$Revision: $"

#ifndef SERVERID_H
#define SERVERID_H

#include <QDialog>
#include "ui_ServerId.h"

class ServerId : public QDialog, public Ui::ServerIdClass
{
	Q_OBJECT

public:
	ServerId(QString * endereco, quint16 * porta, QWidget *parent = 0);
	~ServerId();
	void accept();		// reimplementação

public:
	signals:
	void endereco(QString& end);
	void porta(int porta);

private:
	Ui::ServerIdClass ui;

private:
	QString	mEndereco;
	QString	mPorta;
};

#endif // SERVERID_H
