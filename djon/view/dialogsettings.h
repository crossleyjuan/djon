#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>

namespace Ui {
    class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = 0);
    ~DialogSettings();
    void done(int res);

protected:
    void changeEvent(QEvent *e);

private:
    void populateLogOrderBy();
    void loadCalendars();

    void populateCheckUpdate();
    Ui::DialogSettings *ui;


private slots:
    void on_DialogSettings_accepted();
};

#endif // DIALOGSETTINGS_H
