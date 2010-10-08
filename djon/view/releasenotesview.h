#ifndef RELEASENOTESVIEW_H
#define RELEASENOTESVIEW_H

#include <QDialog>

namespace Ui {
    class ReleaseNotesView;
}

class ReleaseNotesView : public QDialog {
    Q_OBJECT
public:
    ReleaseNotesView(QWidget *parent = 0);
    ~ReleaseNotesView();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ReleaseNotesView *ui;
};

#endif // RELEASENOTESVIEW_H
