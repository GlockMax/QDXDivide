Пример построен на базе стандартного MFC Application, созданного с помощью AppWizard c Application type: Single document и Project style: Visual Studio.

Для запуска примера скопируйте папку NCMExt из места установки NCManager в эту папку. Платформа, для которой вы строите приложение (x64 или Win32), должна соответствовать платформе установленного NCManager.

Вызов функций NCManager осуществляется через меню MainMenu->NCManager.

Весь код, осуществляющий интеграцию, содержится в методах CHostNCMView::OnInitialUpdate, CHostNCMView::OnDestroy, CHostNCMView::OnSize, CMainFrame::OnCopyData и обработчиках элементов меню MainMenu->NCManager.

Подробности см. в \NCMExt\NCMExt.h и \Doc\NCMExt.rtf.
