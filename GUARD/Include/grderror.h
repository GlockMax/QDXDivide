//
// (C) 1997-2005 Aktiv Co. All rights reserved.
// Definitions for use Guardant Stealth.
//       Error codes.
//

#ifndef __GRDERROR__H_
#define __GRDERROR__H_

#ifdef      RUS
#define nv_pszErrors    aszGrdErrorRus
#else       // RUS
#define nv_pszErrors    aszGrdErrorEng
#endif      // RUS

char* aszGrdErrorRus[GrdE_LastError + 1] =
    //- Error codes ( Russian )
{
    "Нет ошибок",                                                                                                        // 00
    "Ключ с заданными условиями поиска не найден",                                                                       // 01
    "Неверный код доступа",                                                                                              // 02
    "Указан слишком большой адрес",                                                                                      // 03
    "Счетчик байт в блоковой опеpации слишком велик",                                                                    // 04
    "Счетчик GP достиг 0",                                                                                               // 05
    "Невеpная команда",                                                                                                  // 06
    "Тайм-аут пpи записи",                                                                                               // 07
    "Ошибка верификации",                                                                                                // 08
    "Сетевой пpотокол не найден",                                                                                        // 09
    "Сетевой pесуpс пpогpаммы исчеpпан",                                                                                 // 10
    "Соединение с сервером Guardant Net потеpяно",                                                                       // 11
    "Сервер Guardant Net не найден",                                                                                     // 12
    "Ошибка распределения памяти в сервере Guardant Net",                                                                // 13
    "Ошибка DPMI",                                                                                                       // 14
    "Внутpенняя ошибка cеpвеpа Guardant Net",                                                                            // 15
    "Сеpвеp Guardant Net был пеpезагpужен",                                                                              // 16
    "Команда не поддеpживается с этим ключом",                                                                           // 17
    "NT дpайвеp не найден",                                                                                              // 18
    "Ошибка сетевого протокола",                                                                                         // 19
    "Получен пакет недопустимого формата",                                                                               // 20
    "Вначале необходимо выполнить Login",                                                                                // 21
    "Вначале необходимо выполнить Logout",                                                                               // 22
    "Этот ключ сейчас заблокирован",                                                                                     // 23
    "Драйвер не может занять LPT порт",                                                                                  // 24
    "Зарезервировано #25",                                                                                               // 25
    "Зарезервировано #26",                                                                                               // 26
    "Зарезервировано #27",                                                                                               // 27
    "Зарезервировано #28",                                                                                               // 28
    "Зарезервировано #29",                                                                                               // 29
    "Ошибка CRC",                                                                                                        // 30
    "Ошибка CRC при чтении",                                                                                             // 31
    "Ошибка CRC при записи",                                                                                             // 32
    "Попытка выйти за границы памяти",                                                                                   // 33
    "Не найдено алгоpитма с запрошенным номером",                                                                        // 34
    "Ошибка CRC при выполнении алгоpитма",                                                                               // 35
    "Ошибка CRC при выполнении ChkNSK или нет больше ключей",                                                            // 36
    "Клиент NSK устаpел для данного ключа",                                                                              // 37
    "Задан ошибочный тип взаимoобpатного пpеобpазования",                                                                // 38
    "Неизвестная ошибка при работе с алгоритмом/ячейкой, операция могла не завершиться",                                 // 39
    "Неверный пароль активации",                                                                                         // 40
    "Достигнут предел максимально допустимого количества ошибок",                                                        // 41
    "Для данного алгоритма/ячейки не предусмотрен данный вид сервиса",                                                   // 42
    "Алгоритм/ячейка находятся в состоянии Inactive, команда не выполнена",                                              // 43
    "Попытка выполнить операцию, которую не поддерживает текущий ключ-сервер",                                           // 44
    "В данный момент ключ не может выполнять никаких операций",                                                          // 45
    "Недопустимые параметры при вызове функции",                                                                         // 46
    "Ошибка распределения памяти",                                                                                       // 47
    "Недопустимый хендл",                                                                                                // 48
    "Этот защищенный контейнер уже используется",                                                                        // 49
    "Зарезервировано #50",                                                                                               // 50
    "Зарезервировано #51",                                                                                               // 51
    "Зарезервировано #52",                                                                                               // 52
    "Нарушена целостность системных данных",                                                                             // 53
    "Вопрос для удаленного обновления не был сгенерирован",                                                              // 54
    "Недопустимый формат данных для удаленного обновления",                                                              // 55
    "Вопрос для удаленного обновления уже сгенерирован",                                                                 // 56
    "Процедура записи при удаленном обновлении не завершена",                                                            // 57
    "Неверное значение хеша данных удаленного обновления",                                                               // 58
    "Внутренняя ошибка",                                                                                                 // 59
    "Данная копия Guardant API уже проинициализирована",                                                                 // 60
    "Ошибка таймера",                                                                                                    // 61
    "Ошибка батареи таймера",                                                                                            // 62
    "Совпадают имена ячеек/алгоритмов в таблице имен ART",                                                               // 63
    "Адреса в таблице AAT за пределами EEPROM",                                                                          // 64
    "Сессионный ключ не сгенерирован",                                                                                   // 65
    "Неверен открый ключ",                                                                                               // 66
    "Неверная цифровая подпись",                                                                                         // 67
    "Генерация ключа данного типа запрещена",                                                                            // 68
    "Сессионный ключ отличается от заданного",                                                                           // 69
    "Сессионный ключ устарел",                                                                                           // 70
    "Вначале необходимо проинициализировать Guardant API",                                                               // 71
    "Неудачная проверка загруженного кода",                                                                              // 72
    "Загруженный код не был выполнен за отведенный таймаут",                                                             // 73
    "В дескрипторе выделен недостаточный размер flash-памяти для пользовательского приложения",                          // 74
    "Зарезервировано #75",                                                                                               // 75
    "Зарезервировано #76",                                                                                               // 76
    "Зарезервировано #77",                                                                                               // 77
    "Зарезервировано #78",                                                                                               // 78
    "Зарезервировано #79",                                                                                               // 79
    "Размер определителя меньше размера структуры TGrdLoadableCodeData",                                                 // 80
    "Некорректно задана область RAM-памяти в загружаемом дескрипторе",                                                   // 81
    "Некорректно задана область FLASH-памяти в загружаемом дескрипторе",                                                 // 82
    "Пересечение областей FLASH-памяти заданных в нескольких дескрипторах",                                              // 83
    "Слишком длинный BMAP файл",                                                                                         // 84
    "Загрузка программы нулевого размера",                                                                               // 85
    "Ошибка при проверке данных",                                                                                        // 86
    "Ошибка протокола при выполнении",                                                                                   // 87
    "Нет загруженной программы пользователя",                                                                            // 88
    "Объявленный в дескрипторе буфер ввода/вывода недостаточен для передачи/приема данных программе пользователя",       // 89
    "При выполнении кода произошло нарушение защиты виртуальной среды",                                                  // 90
    "Буфер ввода/вывода, заданный в программе пользователя, выходит за допустимую область памяти",                       // 91
    "Выход за пределы допустимой области FLASH-памяти.",                                                                 // 92
    "Адресное пространство загружаемой программы пользователя пересекается с уже загруженной (требуется операция Init)", // 93
    "Некорректный формат файла GCEXE",                                                                                   // 94
    "Задана недопустимая область ОЗУ загружаемого кода для вызова GcaCodeRun",                                           // 95
    "Слишком много вложенных вызовов GcaCodeRun",                                                                        // 96
    "Не удалось создать конфигурационный файл клиента",                                                                  // 97
    "Неизвестная ошибка #"                                                                                               // 98 (all errors >= GrdE_LastError )
};

char* aszGrdErrorEng[GrdE_LastError + 1] =
                                                                                                             //- Error codes ( English )
{
    "No errors",                                                                                             // 00
    "Dongle with specified search conditions not found",                                                     // 01
    "Code not found",                                                                                        // 02
    "The specified address is too big",                                                                      // 03
    "Byte counter too big",                                                                                  // 04
    "GP executions counter exhausted (has 0 value)",                                                         // 05
    "Invalid dongle call command",                                                                           // 06
    "Write time out error",                                                                                  // 07
    "Write verification error",                                                                              // 08
    "Network protocol not found",                                                                            // 09
    "License counter of Guardant Net exhausted",                                                             // 10
    "Connection with Guardant Net server was lost",                                                          // 11
    "Guardant Net server not found",                                                                         // 12
    "Guardant Net server memory allocation error",                                                           // 13
    "Guardant Net server found DPMI error",                                                                  // 14
    "Guardant Net server internal error",                                                                    // 15
    "Guardant Net server has been reloaded",                                                                 // 16
    "This command is not supported by this dongle version (the version is too old)",                         // 17
    "Windows NT driver is required",                                                                         // 18
    "Network protocol error",                                                                                // 19
    "Network packet format is not supported",                                                                // 20
    "Logging in Guardant dongle is required",                                                                // 21
    "Logging out from Guardant dongle is required",                                                          // 22
    "Guardant dongle locked by another copy of protected application",                                       // 23
    "Guardant driver cannot capture the parallel port",                                                      // 24
    "Reserved #25",                                                                                          // 25
    "Reserved #26",                                                                                          // 26
    "Reserved #27",                                                                                          // 27
    "Reserved #28",                                                                                          // 28
    "Reserved #29",                                                                                          // 29
    "CRC error occurred while attempting to call the dongle",                                                // 30
    "CRC error occurred while attempting to read data from the dongle",                                      // 31
    "CRC error occurred while attempting to write data to the dongle",                                       // 32
    "The boundary of the dongle's memory has been override",                                                 // 33
    "The hardware algorithm with specified number has not been found in the dongle",                         // 34
    "CRC error of the hardware algorithm",                                                                   // 35
    "All dongles found, or CRC error occurred while attempting to execute ChkNSK operation",                 // 36
    "Guardant API release is too old",                                                                       // 37
    "Non-existent reversible conversion method has been specified",                                          // 38
    "Unknown error in work with algorithm or protected item, operation may by not complete",                 // 39
    "Invalid activation password",                                                                           // 40
    "Error counter exhaust",                                                                                 // 41
    "Specifed algorithm or protected item not support requested service",                                    // 42
    "This is a inactive algorithm or protected item, command not executed",                                  // 43
    "Dongle server not support specifed command",                                                            // 44
    "Dongle busy at this moment and can't execute any command",                                              // 45
    "One or more function arguments are invalid",                                                            // 46
    "Memory allocation error",                                                                               // 47
    "Invalid handle",                                                                                        // 48
    "This protected container is already in use",                                                            // 49
    "Reserved #50",                                                                                          // 50
    "Reserved #51",                                                                                          // 51
    "Reserved #52",                                                                                          // 52
    "Remote update system data corrupted",                                                                   // 53
    "Remote update question has not been generated",                                                         // 54
    "Invalid remote update data format",                                                                     // 55
    "Remote update question has been already generated",                                                     // 56
    "Remote update writing has not been completed",                                                          // 57
    "Invalid remote update data hash",                                                                       // 58
    "Internal error",                                                                                        // 59
    "This copy of Guardant API has been already initialized",                                                // 60
    "Real Time Clock error",                                                                                 // 61
    "Real Time Clock battery low error",                                                                     // 62
    "Duplicate items/algorithms names",                                                                      // 63
    "Address in AAT table is out of range",                                                                  // 64
    "Session key not generated",                                                                             // 65
    "Invalid public key",                                                                                    // 66
    "Invalid digital sign",                                                                                  // 67
    "Session key generation error",                                                                          // 68
    "Invalid session key",                                                                                   // 69
    "Session key too old",                                                                                   // 70
    "Guardant API must be initialized",                                                                      // 71
    "Verification of loadable code failed",                                                                  // 72
    "Loadable code execute time out",                                                                        // 73
    "Flash-size from descriptor for User Firmware too small",                                                // 74
    "Reserved #75",                                                                                          // 75
    "Reserved #76",                                                                                          // 76
    "Reserved #77",                                                                                          // 77
    "Reserved #78",                                                                                          // 78
    "Reserved #79",                                                                                          // 79
    "TGrdLoadableCodeData structure exceeds space reserved for determinant in item descriptor",              // 80
    "Incorrect RAM area specified in loadable code descriptor",                                              // 81
    "Incorrect FLASH memory area specified in loadable code descriptor",                                     // 82
    "Allocation of intersecting FLASH memory areas for different loadable code modules",                     // 83
    "BMAP file is too long",                                                                                 // 84
    "The loadable code has zero length",                                                                     // 85
    "Data verification failed",                                                                              // 86
    "Error in Guardant Code protocol",                                                                       // 87
    "Loadable code not found",                                                                               // 88
    "IO buffer size specified in loadable code is not enough for transmitting/receiving data",               // 89
    "Security violation in Guardant Code virtual environment",                                               // 90
    "IO buffer specified in loadable code exceeds the bounds of allowed RAM area",                           // 91
    "Loadable code exceeds the bounds of allowed FLASH memory area",                                         // 92
    "Allocation of intersecting RAM areas for different loadable code modules (Init operation is required)", // 93
    "Incorrect GCEXE file format",                                                                           // 94
    "Incorrect RAM area specified in loadable code for GcaCodeRun",                                          // 95
    "Too many nested calls of GcaCodeRun",                                                                   // 96
    "Unable to create client configuration file",                                                            // 97
    "Unknown error #"                                                                                        // 98 (all errors >= GrdE_LastError )
};

#endif // !__GRDERROR__H_
