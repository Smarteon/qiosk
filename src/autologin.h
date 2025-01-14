#ifndef AUTOLOGIN_H
#define AUTOLOGIN_H

#include <QWebEngineView>
#include "configuration.h"

extern void injectAutoLoginJS(QWebEngineView *webView, Configuration *config);

#endif // AUTOLOGIN_H
