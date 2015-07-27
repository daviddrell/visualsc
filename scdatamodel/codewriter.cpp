#include "codewriter.h"
#include <QDebug>

CodeWriter::CodeWriter(QString cFileName, QString hFileName):
    cFile(cFileName),
    hFile(hFileName),
    cOut(&cFile),
    hOut(&hFile)
{
    QStringList qsl = cFileName.split("/");
    QString fName = qsl.at(qsl.size()-1);
    className = fName.mid(0,fName.size()-4);    // get the class name out of the entire file name
    qDebug() << "the class name is " << className;
   // cOut = new QTextStream(&cFile);
  //  hOut = new QTextStream(&hFile);
}

bool CodeWriter::helloWorld()
{
    if(!cFile.open(QIODevice::WriteOnly|QIODevice::Text))
        return false;

    if(!hFile.open(QIODevice::WriteOnly|QIODevice::Text))
        return false;

    cOut << "//This is a cpp file\n\nvoid helloWorld()\n{\n\n}\n";

    hOut << "//This is an h file\n\nvoid helloWorld()\n{\n\n}\n";

    return true;
}

bool CodeWriter::writeCppFile()
{
    if(!cFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "ERROR writing to .cpp file!";
        return false;
    }

    // write a code generated message
    cOut << "// This .cpp file was generated using VisualSC's Export Code Function\n\n";

    // write include statement
    cOut << "#include \"" << className << ".h\"\n\n";

    // write constructor
    cOut << className <<"::"<<className<<"()\n{\n\n}\n";

    // write deconstructor
    cOut << className <<"::~"<<className<<"()\n{\n\n}\n";


    // true if no complications
    return true;
}

bool CodeWriter::writeHFile()
{
    if(!hFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "ERROR writing to .h file!";
        return false;
    }

    // write a code generated message
    hOut << "// This header file was generated using VisualSC's Export Code Function\n\n";

    // write ifndef and define
    hOut << "#ifndef "<< className.toUpper() << "_H\n#define "<<className.toUpper()<<"_H\n\n";

    // write class shell
    hOut << "class " << className <<"\n{\npublic:\n\t"<<className<<"();\n\t~"<<className<<"();\n\nprivate:\n\n};\n\n";


    // write endif
    hOut << "#endif // "<<className.toUpper()<<"_H\n";


    // true if no complications
    return true;

}



CodeWriter::~CodeWriter()
{

}

