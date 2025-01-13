#include "autologin.h"

void injectAutoLoginJS(QWebEngineView *webView, Configuration *config) {
    QString js = QString(R"(
        function fillCredentialsAndLogin() {
            var usernameField = document.querySelector('#username');
            var passwordField = document.querySelector('#password');
            var loginButton = document.querySelector('.login-form__connect-button');

            if (usernameField) {
                usernameField.value = '%1';
            }
            if (passwordField) {
                passwordField.value = '%2';
            }

            if (loginButton) {
                tryClickButton(loginButton);
            }
        }

        function tryClickButton(button, attempts = 0) {
            if (attempts >= 10) {  // Max 10 attempts (50 seconds)
                console.log('Failed to click login button after multiple attempts');
                return;
            }

            if (!button.disabled && button.offsetParent !== null) {
                button.click();
            } else {
                // Button is not clickable yet, retry after a delay
                setTimeout(() => tryClickButton(button, attempts + 1), 5000);  // Retry every 5 seconds
            }
        }

        function observeDocument() {
            var observer = new MutationObserver(function(mutations) {
                mutations.forEach(function(mutation) {
                    if (mutation.type === 'childList') {
                        fillCredentialsAndLogin();
                    }
                });
            });

            var config = { childList: true, subtree: true };
            observer.observe(document.body, config);
        }

        document.fonts.ready.then(function () {
          document.body.style.visibility = 'visible';
        });
        document.body.style.visibility = 'hidden';

        if (document.readyState === 'loading') {
            document.addEventListener('DOMContentLoaded', observeDocument);
        } else {
            observeDocument();
        }

        // Initial fill and login attempt in case elements are already present
        fillCredentialsAndLogin();
    )").arg(config->getAutologinUsername(), config->getAutologinPassword());

    webView->page()->runJavaScript(js);
}
