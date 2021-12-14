SingleApplicationHelper
=================
A cross-platform Qt library. Keeps the first Instance of your Application and kills each subsequent instances. It can send data to the primary instance from secondary instances.

Usage
-----

The `SingleApplicationHelper` class does not inherits from whatever `Q[Core|Gui]Application` class.

So, you can use the library without modifying `Q[Core|Gui]Application` class.

```cpp
#include <QApplication>
#include <QMainWindow>
#include "SingleApplicationHelper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("MyApp");

    QMainWindow w;

    SingleApplicationHelper singleAppHelper;
    // The first application will register the server successfully, and listen for connections. 
    // The second try to register the server, but failed.
    if (singleAppHelper.registerApplication(a.applicationName(), SingleApplicationHelper::User, 500)) {
        QObject::connect(&singleAppHelper, &SingleApplicationHelper::sigSecondApplicationArguments,
                         &w, &QMainWindow::raise);
        w.show();
    } else {
    // The second application can send the arguments to first application by the method below.    
        singleAppHelper.sendSecondApplicationArguments(a.applicationName(), "--show", 500);
        return 0;
    }

    return a.exec();
}
```

**CMake:**

Include the subdirectory in your `CMakeLists.txt` project file.

```cmake
add_subdirectory(src/third-party/SingleApplicationHelper)
target_link_libraries(${PROJECT_NAME} SingleApplicationHelper)
```

API
---

### Members

```cpp
bool SingleApplicationHelper::registerApplication(const QString &appName, Options options = Mode::User, int timeout = 300)
```

The first instance registers the local server successfully and the second failed.

- appName - The server name that the socket can connect and send data. I recommend using the application name.
- options - server access restricted, see  **Flags** for detail.
- timeout - The time that socket waits for connected.

---

```
void sendSecondApplicationArguments(const QString &appName, const QString &arguments, int timeout = 300)
```

The second instance can call the method to send arguments to the first instance.

- appName - The server name that the socket will connect and send data.
- arguments - The arguments that second instance wants to tell the first instance.
- timeout -   The time that socket waits for connected.

---

### Signals

```cpp
void sigSecondApplicationArguments(QString arguments);
```

Triggered whenever a new instance had been started. 

---

### Flags

```cpp
enum SingleApplicationHelper::Mode
```

*   `Mode::User` - The SingleApplicationHelper should apply user wide. Access is restricted to the same user as the process that created the server.  This is the default functionality.
*   `Mode::System` - The SingleApplicationHelper applies system-wide, no access restrictions.

---

Implementation
--------------

The library sets up a `QLocalServer`by the first instance of your application, and listen for connections. The second instance will be failed. The second instance can send message to first instance by socket.

License
-------
This library is released under `The MIT License (MIT)`.

## Thanks

Thanks to [SingleApplication](https://github.com/itay-grudev/SingleApplication) of [itay-grudev](https://github.com/itay-grudev), I just simplify implementation with QLocalServer/QLocalSocket.

