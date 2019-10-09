#include "./desktoputils.h"

#include <QDesktopServices>
#include <QUrl>

#ifdef Q_OS_WIN32
#include <filesystem>
#endif

namespace QtUtilities {

/*!
 * \brief Shows the specified file or directory using the default file browser.
 * \remarks
 * - The specified \a path must *not* be specified as URL. (The conversion to a URL suitable for
 *   QDesktopServices::openUrl() is the whole purpose of this function).
 * - The Qt documentation suggests to use
 *   `QDesktopServices::openUrl(QUrl("file:///C:/Documents and Settings/All Users/Desktop", QUrl::TolerantMode));`
 *   under Windows. However, that does not work if the path contains a '#'. It is also better to use
 *   QUrl::DecodedMode to prevent QUrl from interpreting any of the paths characters in a special way.
 */
bool openLocalFileOrDir(const QString &path)
{
    QUrl url(QStringLiteral("file://"));

#ifdef Q_OS_WIN32
    // replace backslashes with regular slashes
    QString tmp(path);
    tmp.replace(QChar('\\'), QChar('/'));

    // add a slash before the drive letter of an absolute path
    if (std::filesystem::path(path.toStdString()).is_absolute()) {
        tmp = QStringLiteral("/") + tmp;
    }
    url.setPath(tmp, QUrl::DecodedMode);

#else
    url.setPath(path, QUrl::DecodedMode);
#endif
    return QDesktopServices::openUrl(url);
}
} // namespace QtUtilities
