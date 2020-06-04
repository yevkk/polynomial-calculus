#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../src/Polynomial.hpp"
#include "../src/PolynomialRing.hpp"
#include "../src/PolynomialField.hpp"
#include <QMainWindow>
#include <vector>
#include <memory>

using namespace lab;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_changePBtn_clicked();

    void on_changeNBtn_clicked();

    void on_setupBtn_clicked();

    void on_runFieldBtn_clicked();

    void on_fieldActionSelect_activated(int index);

private:
    Ui::MainWindow *ui;

    void setP(int64_t p);

    void setN(int64_t n);

    void displayIrreducible(const std::vector<Polynomial>& polynomials);

    void showError();

    struct {
        int64_t p;
        int64_t n;
        Polynomial irreducible;
    } _setup;

    std::vector<Polynomial> _irreducible_vector;

    std::unique_ptr<PolynomialRing> _ring;
    std::unique_ptr<PolynomialField> _field;
};
#endif // MAINWINDOW_H
