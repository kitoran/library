﻿#ifndef SHITTYINTROSPECTION_H
#define SHITTYINTROSPECTION_H
#ifndef NO_SHITTY_INTROSPECTION
#define _NUM_ARGS(X50, X49, X48, X47, X46, X45, X44, X43, X42, X41, X40, X39, X38, X37, X36, X35, X34, X33, X32, X31, X30, X29, X28, X27, X26, X25, X24, X23, X22, X21, X20, X19, X18, X17, X16, X15, X14, X13, X12, X11, X10, X9, X8, X7, X6, X5, X4, X3, X2, X1, N, ...)   N

#define NUM_ARGS(...) _NUM_ARGS(__VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define EXPAND(X)       X
#define FIRSTARG(X, ...)    (X)
#define RESTARGS(X, ...)    (__VA_ARGS__)
#define FOREACH(MACRO, LIST)    FOREACH_(NUM_ARGS LIST, MACRO, LIST)
#define FOREACH_(N, M, LIST)    FOREACH__(N, M, LIST)
#define FOREACH__(N, M, LIST)   FOREACH_##N(M, LIST)
#define FOREACH_1(M, LIST)  M LIST
#define FOREACH_2(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_1(M, RESTARGS LIST)
#define FOREACH_3(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_2(M, RESTARGS LIST)
#define FOREACH_4(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_3(M, RESTARGS LIST)
#define FOREACH_5(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_4(M, RESTARGS LIST)
#define FOREACH_6(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_5(M, RESTARGS LIST)
#define FOREACH_7(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_6(M, RESTARGS LIST)
#define FOREACH_8(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_7(M, RESTARGS LIST)
#define FOREACH_9(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_8(M, RESTARGS LIST)
#define FOREACH_10(M, LIST) EXPAND(M FIRSTARG LIST) FOREACH_9(M, RESTARGS LIST)
#define FOREACH_11(M, LIST) EXPAND(M FIRSTARG LIST) FOREACH_10(M, RESTARGS LIST)
#define FOREACH_12(M, LIST) EXPAND(M FIRSTARG LIST) FOREACH_11(M, RESTARGS LIST)
#define STRINGIFY_COMMA(X)    #X,

#define FIRSTARGS2(X, Y, ...)    (X, Y)
#define RESTARGS2(X, Y, ...)    (__VA_ARGS__)
#define FOREACH2(MACRO, LIST)    FOREACH2_(NUM_ARGS LIST, MACRO, LIST)
#define FOREACH2_(N, M, LIST)    FOREACH2__(N, M, LIST)
#define FOREACH2__(N, M, LIST)   FOREACH2_##N(M, LIST)
#define FOREACH2_2(M, LIST)  M LIST
#define FOREACH2_4(M, LIST)  EXPAND(M FIRSTARGS2 LIST) FOREACH2_2(M, RESTARGS2 LIST)
#define FOREACH2_6(M, LIST)  EXPAND(M FIRSTARGS2 LIST) FOREACH2_4(M, RESTARGS2 LIST)
#define FOREACH2_8(M, LIST)  EXPAND(M FIRSTARGS2 LIST) FOREACH2_6(M, RESTARGS2 LIST)
#define FOREACH2_10(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_8(M, RESTARGS2 LIST)
#define FOREACH2_12(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_10(M, RESTARGS2 LIST)
#define FOREACH2_14(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_12(M, RESTARGS2 LIST)
#define FOREACH2_16(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_14(M, RESTARGS2 LIST)
#define FOREACH2_18(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_16(M, RESTARGS2 LIST)
#define FOREACH2_20(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_18(M, RESTARGS2 LIST)
#define FOREACH2_22(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_20(M, RESTARGS2 LIST)
#define FOREACH2_24(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_22(M, RESTARGS2 LIST)
#define FOREACH2_26(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_24(M, RESTARGS2 LIST)
#define FOREACH2_28(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_26(M, RESTARGS2 LIST)
#define FOREACH2_30(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_28(M, RESTARGS2 LIST)
#define FOREACH2_32(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_30(M, RESTARGS2 LIST)
#define FOREACH2_34(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_32(M, RESTARGS2 LIST)
#define FOREACH2_36(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_34(M, RESTARGS2 LIST)
#define FOREACH2_38(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_36(M, RESTARGS2 LIST)
#define FOREACH2_40(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_38(M, RESTARGS2 LIST)
#define FOREACH2_42(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_40(M, RESTARGS2 LIST)
#define FOREACH2_44(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_42(M, RESTARGS2 LIST)
#define FOREACH2_46(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_44(M, RESTARGS2 LIST)
#define FOREACH2_48(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_46(M, RESTARGS2 LIST)
#define FOREACH2_50(M, LIST) EXPAND(M FIRSTARGS2 LIST) FOREACH2_48(M, RESTARGS2 LIST)
#define FIRST(X, Y) X,
#define STRINGIFY_COMMA_SECOND(X, Y)    #Y,
//#define SECOND(X, Y)    Y
//#define FITSTS2(...)
//typedef arrayOfStrings;
#define INTROSPECT_ENUM(Name, ...) \
    typedef const char*const (The ## Name ## Type)[NUM_ARGS(__VA_ARGS__)+1]; \
    static  The ## Name ## Type Name ## Names  = {FOREACH(STRINGIFY_COMMA, (__VA_ARGS__))}; \
    typedef enum {__VA_ARGS__} Name; \
    static const Name Name ## Enumerators[NUM_ARGS(__VA_ARGS__)] = {__VA_ARGS__}; \
    static const int Name ## Size = NUM_ARGS(__VA_ARGS__);

#define INTROSPECT_ENUM_VISIBLE_NAMES(Name, ...) constexpr char const*const Name ## Names [NUM_ARGS(__VA_ARGS__)/2] = {FOREACH2(STRINGIFY_COMMA_SECOND, (__VA_ARGS__))}; \
    enum Name {FOREACH2(FIRST, (__VA_ARGS__))}; \
    const Name Name ## Enumerators[NUM_ARGS(__VA_ARGS__)/2] = {FOREACH2(FIRST, (__VA_ARGS__))}; \
    const int Name ## Size = NUM_ARGS(__VA_ARGS__)/2;

//#define INTROSPECT_ENUM_PERSISTENT_COMBOBOX(Name, ...) INTROSPECT_ENUM(Name, __VA_ARGS__)
//    bool Name ## PersistentComboBox (Painter *p, Name* current, char* name) {
//        return persistentComboBoxZT_(p, Name ## Names, current, name);
//    }
#define persistentEnumComboBox(enum, p, c) \
    persistentComboBoxZT_(p, enum ## Names, c, #c)
//#define INTROSPECT_ENUM_VISIBLE_NAMES_PERSISTENT_COMBOBOX(Name, ...) INTROSPECT_ENUM_VISIBLE_NAMES(Name, __VA_ARGS__)
//    class Name ## ComboBox : public QComboBox
//    {
//        Q_OBJECT
//    public:
//        explicit Name ## ComboBox (QWidget *parent = 0):QComboBox(parent) {
//            for(Name name : Name ## Enumerators) {
//                addItem(Name ## Names[name], name);
//            }
//            connect(this, &QComboBox::objectNameChanged, this, & Name ## ComboBox::restoreSetting);
//        }
//        ~Name ## ComboBox() {
//            QSettings().setValue(objectName(), currentText());
//        }
//    private slots:
//        void restoreSetting(const QString& name) {
//            setCurrentText(QSettings().value(name).toString());
//        }
//    };

#define INTROSPECT_ENUM_VISIBLE_NAMES_PERSISTENT_COMBOBOX_ENUMNAME(Name, ...) INTROSPECT_ENUM_VISIBLE_NAMES(Name, __VA_ARGS__) \
    class Name ## ComboBox : public QComboBox           \
    {                                                   \
        Q_OBJECT                                        \
    public:                                             \
        explicit Name ## ComboBox (QWidget *parent = 0):QComboBox(parent) { \
            for(Name name : Name ## Enumerators) {      \
                addItem(Name ## Names[name], name);     \
            }                                           \
            setCurrentIndex(QSettings().value(#Name).toInt()); \
        }                                               \
        explicit Name ## ComboBox (Name* var, QWidget *parent = 0):QComboBox(parent) { \
            for(Name name : Name ## Enumerators) {      \
                addItem(Name ## Names[name], name);     \
            }                                           \
            connect(this, qOverload<int>(&QComboBox::currentIndexChanged), \
                    this, [var](int index){             \
                *var = (Name)index;                     \
            });                                         \
            setCurrentIndex(QSettings().value(#Name).toInt()); \
        }                                               \
        ~Name ## ComboBox() {                           \
            QSettings().setValue(#Name, 0); \
        }                                               \
    };

#define INTROSPECT_ENUM_VISIBLE_NAMES_COMBOBOX(Name, ...) INTROSPECT_ENUM_VISIBLE_NAMES(Name, __VA_ARGS__) \
    class Name ## ComboBox : public QComboBox           \
    {                                                   \
        Q_OBJECT                                        \
    public:                                             \
        explicit Name ## ComboBox (QWidget *parent = 0):QComboBox(parent) { \
            for(Name name : Name ## Enumerators) {      \
                addItem(Name ## Names[name], name);     \
            }                                           \
        }                                               \
    };


#define DECLARE_QDEBUG_OP(Name) QDebug operator<<(QDebug, Name);
#define QDEBUG_OP(Name) QDebug operator<<(QDebug debug, Name n) { \
    using namespace std; \
    QDebugStateSaver saver(debug); Q_UNUSED(saver) \
    if(n > 0 && n < Name ## Size) { \
        debug.nospace().noquote() << Name ## Names [n]; \
    } else { \
        debug.nospace().noquote() << "invalid " #Name "(" << static_cast< \
                conditional<is_same<underlying_type<Name>::type, char>::value, int, \
                conditional<is_same<underlying_type<Name>::type, unsigned char>::value, unsigned int, \
                underlying_type<Name>::type>::type>::type>(n) << ")"; \
    } \
    return debug; \
}
#else
#define INTROSPECT_ENUM(Name, ...)  enum Name {__VA_ARGS__};
#endif
#endif // SHITTYINTROSPECTION_H
