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

    ui->stackedWidget->setCurrentIndex(0);

    on_fieldActionSelect_activated(0);
    on_ringActionSelect_activated(0);
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
    ui->statusbar->showMessage("p: " + QString::number(_setup.p) + ", n: " + QString::number(_setup.n) + "; " + QString::fromStdString(to_string(_setup.irreducible)));

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
    default:
        break;
    }

    ui->fieldInputInfoLabel->setText(action_info);
}

void MainWindow::on_runFieldBtn_clicked() {
    ui->statusbar->showMessage("p: " + QString::number(_setup.p) + ", n: " + QString::number(_setup.n) + "; " + QString::fromStdString(to_string(_setup.irreducible)));
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

    ui->fieldResultText->setPlainText(QString::fromStdString(result_str));
}


void MainWindow::on_ringActionSelect_activated(int index) {
    ui->statusbar->showMessage("p: " + QString::number(_setup.p) + ", n: " + QString::number(_setup.n) + "; " + QString::fromStdString(to_string(_setup.irreducible)));

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
        action_info = "Polynomial 1: first argument;\nPolynomial 2: second argument;\nNumber: --- ;";
        break;
    case 4:
        action_info = "Polynomial 1: first argument;\nPolynomial 2: second argument;\nNumber: --- ;";
        break;
    case 5:
        action_info = "Polynomial 1: --- ;\nPolynomial 2: --- ;\nNumber: order;";
        break;
    case 6:
        action_info = "Polynomial 1: --- ;\nPolynomial 2: --- ;\nNumber: order;";
        break;
    case 7:
        action_info = "Polynomial 1: polynomial;\nPolynomial 2: --- ;\nNumber: --- ;";
        break;
    case 8:
        action_info = "Polynomial 1: polyonomial ;\nPolynomial 2: --- ;\nNumber: variable value;";
        break;
    case 9:
        action_info = "Polynomial 1: polynomial;\nPolynomial 2: --- ;\nNumber: --- ;";
        break;
    case 10:
        action_info = "Polynomial 1: polynomial;\nPolynomial 2: --- ;\nNumber: --- ;";
        break;
    case 11:
        action_info = "Polynomial 1: polynomial;\nPolynomial 2: --- ;\nNumber: --- ;";
        break;
    case 12:
        action_info = "Polynomial 1: polynomial;\nPolynomial 2: --- ;\nNumber: --- ;";
        break;
    default:
        break;
    }

    ui->ringInputInfoLabel->setText(action_info);
}

void MainWindow::on_runRingBtn_clicked() {
    ui->statusbar->showMessage("p: " + QString::number(_setup.p) + ", n: " + QString::number(_setup.n) + "; " + QString::fromStdString(to_string(_setup.irreducible)));

    int index = ui->ringActionSelect->currentIndex();
    auto pol_str1 = ui->polynomial1RingLine->text();
    auto pol_str2 = ui->polynomial2RingLine->text();
    auto num_str = ui->numRingLine->text();

    std::string result_str;

    switch (index) {
    case 0: {
        auto left = detail_ui::from_qstring(pol_str1);
        auto right = detail_ui::from_qstring(pol_str2);

        result_str = to_string(_ring->add(left, right));

        break;
    }

    case 1: {
        auto left = detail_ui::from_qstring(pol_str1);
        auto right = detail_ui::from_qstring(pol_str2);

        result_str = to_string(_ring->subtract(left, right));

        break;
    }

    case 2: {
        auto left = detail_ui::from_qstring(pol_str1);
        auto right = detail_ui::from_qstring(pol_str2);

        result_str = to_string(_ring->multiply(left, right));

        break;
    }

    case 3: {
        auto left = detail_ui::from_qstring(pol_str1);
        auto right = detail_ui::from_qstring(pol_str2);

        if (right == Polynomial{0}) {
            showError();
            return;
        }

        auto result = _ring->div_mod(left, right);

        result_str = "div: " + to_string(result.first) + "\nmod: " + to_string(result.second);

        break;
    }

    case 4: {
        auto left = detail_ui::from_qstring(pol_str1);
        auto right = detail_ui::from_qstring(pol_str2);

        result_str = to_string(_ring->gcd(left, right));

        break;
    }

    case 5: {
        int num = num_str.toInt();

        result_str = to_string(_ring->cyclotomicPolinomial(num));

        break;
    }

    case 6: {
        int num = num_str.toInt();

        auto result = _ring->cyclotomicFactorization(num);

        result_str = "(" + to_string(result[0]) + ")";
        for (size_t i = 1; i < result.size(); i++) {
             result_str += "*(" + to_string(result[1]) + ")";
        }

        break;
    }

    case 7: {
        auto left = detail_ui::from_qstring(pol_str1);

        result_str = to_string(_ring->normalize(left));

        break;
    }

    case 8: {
        auto left = detail_ui::from_qstring(pol_str1);
        int num = num_str.toInt();

        result_str = std::to_string(_ring->evaluate(left, num));

        break;
    }

    case 9: {
        auto left = detail_ui::from_qstring(pol_str1);

        result_str = to_string(_ring->derivate(left));

        break;
    }

    case 10: {
        auto left = detail_ui::from_qstring(pol_str1);

        result_str = std::to_string(_ring->countRoots(left, PolynomialRing::CountPolicy::GCD));

        break;
    }

    case 11: {
        auto left = detail_ui::from_qstring(pol_str1);

        result_str = std::to_string(_ring->isIrreducible(left));

        break;
    }

    case 12: {
        auto left = detail_ui::from_qstring(pol_str1);
          qDebug() << QString("w");
        if (!_ring->isIrreducible(left)) {
            showError();
            return;
        }

        result_str = std::to_string(_ring->order_of_irreducible(left));

        break;
    }

    default:
        break;
    }

    ui->ringResultText->setPlainText(QString::fromStdString(result_str));
}

void MainWindow::on_actionReset_triggered() {
    setP(2);
    setN(2);

    _irreducible_vector = _ring->irreducibleOfOrder(_setup.n);
    displayIrreducible(_irreducible_vector);

    ui->stackedWidget->setCurrentIndex(0);

    on_fieldActionSelect_activated(0);
    on_ringActionSelect_activated(0);

    ui->fieldResultText->clear();
    ui->ringResultText->clear();

    ui->polynomial1FieldLine->clear();
    ui->polynomial2FieldLine->clear();
    ui->numFieldLine->clear();

    ui->polynomial1RingLine->clear();
    ui->polynomial2RingLine->clear();
    ui->numRingLine->clear();
}
