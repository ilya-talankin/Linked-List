import qbs

CppApplication {
    consoleApplication: true
    install: true
    files: [
        "linked_list.c",
        "linked_list.h",
        "main.c",
    ]
}
