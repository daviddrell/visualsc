#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <QTextStream>
#include <QFile>

class CodeWriter
{
public:
    CodeWriter(QString, QString);
    ~CodeWriter();

    bool helloWorld();
    bool writeHFile();
    bool writeCppFile();

private:
    QString className;
    QFile cFile;
    QFile hFile;
    QTextStream cOut;
    QTextStream hOut;


};

#endif // CODEWRITER_H
