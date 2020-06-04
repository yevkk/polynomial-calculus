#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>
#include <QInputDialog>
#include <QMessageBox>

#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);

    setP(2);
    setN(2);

    _irreducible_vector = _ring->irreducibleOfOrder(_setup.n);
    displayIrreducible(_irreducible_vector);

    on_fieldActionSelect_activated(0);
}

MainWindow::~MainWindow() {
    delete ui;
}

namespace detail_ui {
    bool prime(const uint64_t& n){
        for(uint64_t i = 2; i <= sqrt(n); i++) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }

    Polynomial from_qstring(const QString& qstr) {
        std::stringstream iss{qstr.toStdString()};

        int number;
        std::vector<int64_t> coefs;
        while ( iss >> number )
          coefs.push_back( number );

        return Polynomial{coefs};
    }
}

void MainWindow::setP(int64_t p) {
    assert(detail_ui::prime(p) && "p should be prime");
    _setup.p = p;
    _ring = std::make_unique<PolynomialRing>(_setup.p);
    ui->pDisplay->setText(QString::number(p));
}

void MainWindow::setN(int64_t n) {
    assert(n > 0 && "n should be > 0");
    _setup.n = n;
    ui->nDisplay->setText(QString::number(n));
}

void MainWindow::displayIrreducible(const std::vector<Polynomial> &polynomials) {
    auto list = ui->irreducibleList;
    list->clear();

    for (const auto& item : polynomials) {
        list->addItem(QString::fromStdString(to_string(item)));
    }
}

void MainWindow::showError() {
    QMessageBox::warning(this, "Warning", "Incorrect input", QMessageBox::Ok);
}

void MainWindow::on_changePBtn_clicked() {
    bool ok = true;
    int input = 10; //just non-prime number

    while(ok && !detail_ui::prime(input)) {
        input = QInputDialog::getInt(this, "p input", "Enter a prime number:", _setup.p, 0, INT_MAX, 1, &ok);
    }

    setP(input);

    _irreducible_vector = _ring->irreducibleOfOrder(_setup.n);
    displayIrreducible(_irreducible_vector);
}

void MainWindow::on_changeNBtn_clicked() {
    bool ok;

    int input = QInputDialog::getInt(this, "n input", "Enter a positive number:", _setup.n, 1, INT_MAX, 1, &ok);

    if (ok) {
        setN(input);

        _irreducible_vector = _ring->irreducibleOfOrder(_setup.n);
        displayIrreducible(_irreducible_vector);
    }
}

void MainWindow::on_setupBtn_clicked() {
    if (ui->irreducibleList->currentRow() == -1) {
        QMessageBox::warning(this, "Setup", "Select irreducible polynomial first", QMessageBox::Ok);
        return;
    }

    _setup.irreducible = _irreducible_vector[ui->irreducibleList->currentRow()];
    _field = std::make_unique<PolynomialField>(_setup.p, _setup.irreducible);

    ui->stackedWidget->setCurrentIndex(1);
    ui->statusbar->showMessage("p: " + QString::number(_setup.p) + ", n: " + QString::number(_setup.n) + "; " + QString::fromStdString(to_string(_setup.irreducible)));
}

void MainWindow::on_fieldActionSelect_activated(int index) {
    QString action_info;
    switch (index) {
    case 0:
        action_info = "Polynomial 1: first argument;\nPolynomial 2: second argument;\nNumber: --- ;";
        break;
    case 1:
        action_info = "Polynomial 1: first argument;\nPolynomial 2: second argument;\nNumber: --- ;";
        break;
    case 2:
        action_info = "Polynomial 1: first argument;\nPolynomial 2: second argument;\nNumber: --- ;";
        break;
    case 3:
        action_info = "Polynomial 1: argument;\nPolynomial 2: --- ;\nNumber: --- ;";
        break;
    case 4:
        action_info = "Polynomial 1: polynomial;\nPolynomial 2: --- ;\nNumber: power;";
        break;
//    case 5:
//        action_info = "Polynomial 1: first argument;\nPolynomial 2: second argument;\nNumber: --- ;";
//        break;
    default:
        break;
    }

    ui->fieldInputInfoLabel->setText(action_info);
}

void MainWindow::on_runFieldBtn_clicked() {
    int index = ui->fieldActionSelect->currentIndex();
    auto pol_str1 = ui->polynomial1FieldLine->text();
    auto pol_str2 = ui->polynomial2FieldLine->text();
    auto num_str = ui->numFieldLine->text();

    std::string result_str;

    switch (index) {
    case 0: {
        auto left = detail_ui::from_qstring(pol_str1);
        auto right = detail_ui::from_qstring(pol_str2);

        if (!(left.degree() < _setup.n && right.degree() < _setup.n)) {
            showError();
            return;
        }

        result_str = to_string(_field->add(left, right));

        break;
    }

    case 1: {
        auto left = detail_ui::from_qstring(pol_str1);
        auto right = detail_ui::from_qstring(pol_str2);

        if (!(left.degree() < _setup.n && right.degree() < _setup.n)) {
            showError();
            return;
        }

        result_str = to_string(_field->subtract(left, right));

        break;
    }

    case 2: {
        auto left = detail_ui::from_qstring(pol_str1);
        auto right = detail_ui::from_qstring(pol_str2);

        if (!(left.degree() < _setup.n && right.degree() < _setup.n)) {
            showError();
            return;
        }

        result_str = to_string(_field->multiply(left, right));

        break;
    }

    case 3: {
        auto left = detail_ui::from_qstring(pol_str1);

        if (!(left.degree() < _setup.n)) {
            showError();
            return;
        }

        result_str = to_string(_field->inverted(left));

        break;
    }

    case 4: {
        auto left = detail_ui::from_qstring(pol_str1);
        int num = num_str.toInt();

        if (!(left.degree() < _setup.n)) {
            showError();
            return;
        }

        result_str = to_string(_field->pow(left, num));

        break;

    }
    default:
        break;
    }

    ui->fieldResultLabel->setText(QString::fromStdString(result_str));
}

