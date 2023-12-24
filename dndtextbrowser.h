// dndtextbrowser.h
#ifndef DNDTEXTBROWSER_H
#define DNDTEXTBROWSER_H

#include <QTextBrowser>

class DnDTextBrowser : public QTextBrowser {
    Q_OBJECT
public:
    explicit DnDTextBrowser(QWidget *parent = nullptr);
signals:
    void fileDropped(const QString &filePath);

protected:
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};

#endif // DNDTEXTBROWSER_H
