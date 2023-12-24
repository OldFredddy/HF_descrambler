#include "dndtextbrowser.h"
#include <QTextBrowser>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>

DnDTextBrowser::DnDTextBrowser(QWidget *parent) : QTextBrowser(parent) {
    setAcceptDrops(true);
    setEnabled(true); // Это необязательно для QTextBrowser
}

void DnDTextBrowser::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) {  // Проверяем, содержат ли данные URL
           e->acceptProposedAction();
       } else {
           e->ignore(); // Важно для того, чтобы отклонить неподходящие события
       }
}

void DnDTextBrowser::dragMoveEvent(QDragMoveEvent *e) {
    if (e->mimeData()->hasUrls()) {
           e->accept();
       } else {
           e->ignore();
       }
}

void DnDTextBrowser::dropEvent(QDropEvent *e) {
    if (e->mimeData()->hasUrls()) {
           QList<QUrl> urls = e->mimeData()->urls();
           if (!urls.isEmpty()) {
               // Предполагаем, что вам нужен путь к первому файлу
               QString filePath = urls.first().toLocalFile();
               emit fileDropped(filePath);  // Испускаем сигнал с путем к файлу
           }
           e->acceptProposedAction();
       }
}
