#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>
#include <QInputDialog>
#include <QMessageBox>

#include <cmath>
#include <cassert>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);

    setP(2);
    setN(2);

    _irreducible_vector = {Polynomial{10}, Polynomial{12}, Polynomial{14}}; //TODO: _ring->calculateElements(_setup.n);
    displayIrreducible(_irreducible_vector);
}

MainWindow::~MainWindow() {
    delete ui;
}

namespace detail {
    bool prime(const uint64_t& n){
        for(uint64_t i = 2; i <= sqrt(n); i++) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }
}

void MainWindow::setP(int64_t p) {
    assert(detail::prime(p) && "p should be prime");
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


void MainWindow::on_changePBtn_clicked() {
    bool ok = true;
    int input = 10; //just non-prime number

    while(ok && !detail::prime(input)) {
        input = QInputDialog::getInt(this, "p input", "Enter a prime number:", _setup.p, 0, INT_MAX, 1, &ok);
    }

    setP(input);

    _irreducible_vector = {Polynomial{1}, Polynomial{2}, Polynomial{3}}; //TODO: _ring->calculateElements(_setup.n);
    displayIrreducible(_irreducible_vector);
}

void MainWindow::on_changeNBtn_clicked() {
    bool ok;

    int input = QInputDialog::getInt(this, "n input", "Enter a positive number:", _setup.n, 0, INT_MAX, 1, &ok);

    if (ok) {
        setN(input);

        _irreducible_vector = {Polynomial{4}, Polynomial{5}, Polynomial{8}}; //TODO: _ring->calculateElements(_setup.n);
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
}
