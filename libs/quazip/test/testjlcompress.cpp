#include "testjlcompress.h"

#include "qztest.h"

#include <QDir>
#include <QFileInfo>

#include <QtTest/QtTest>

#include <quazip/jlcompress.h>

void TestJlCompress::compressFile_data()
{
    QTest::addColumn<QString>("zipName");
    QTest::addColumn<QString>("fileName");
    QTest::newRow("simple") << "jlsimplefile.zip" << "test0.txt";
}

void TestJlCompress::compressFile()
{
    QFETCH(QString, zipName);
    QFETCH(QString, fileName);
    QDir curDir;
    if (curDir.exists(zipName)) {
        if (!curDir.remove(zipName))
            QFAIL("Can't remove zip file");
    }
    if (!createTestFiles(QStringList() << fileName)) {
        QFAIL("Can't create test file");
    }
    QVERIFY(JlCompress::compressFile(zipName, "tmp/" + fileName));
    // get the file list and check it
    QStringList fileList = JlCompress::getFileList(zipName);
    QCOMPARE(fileList.count(), 1);
    QVERIFY(fileList[0] == fileName);
    removeTestFiles(QStringList() << fileName);
    curDir.remove(zipName);
}

void TestJlCompress::compressFiles_data()
{
    QTest::addColumn<QString>("zipName");
    QTest::addColumn<QStringList>("fileNames");
    QTest::newRow("simple") << "jlsimplefiles.zip" <<
        (QStringList() << "test0.txt" << "test00.txt");
    QTest::newRow("different subdirs") << "jlsubdirfiles.zip" <<
        (QStringList() << "subdir1/test1.txt" << "subdir2/test2.txt");
}

void TestJlCompress::compressFiles()
{
    QFETCH(QString, zipName);
    QFETCH(QStringList, fileNames);
    QDir curDir;
    if (curDir.exists(zipName)) {
        if (!curDir.remove(zipName))
            QFAIL("Can't remove zip file");
    }
    if (!createTestFiles(fileNames)) {
        QFAIL("Can't create test files");
    }
    QStringList realNamesList, shortNamesList;
    foreach (QString fileName, fileNames) {
        QString realName = "tmp/" + fileName;
        realNamesList += realName;
        shortNamesList += QFileInfo(realName).fileName();
    }
    QVERIFY(JlCompress::compressFiles(zipName, realNamesList));
    // get the file list and check it
    QStringList fileList = JlCompress::getFileList(zipName);
    QCOMPARE(fileList, shortNamesList);
    removeTestFiles(fileNames);
    curDir.remove(zipName);
}

void TestJlCompress::compressDir_data()
{
    QTest::addColumn<QString>("zipName");
    QTest::addColumn<QStringList>("fileNames");
    QTest::newRow("simple") << "jldir.zip" << (
            QStringList() << "test0.txt" << "testdir1/test1.txt"
            << "testdir2/test2.txt" << "testdir2/subdir/test2sub.txt");
}

void TestJlCompress::compressDir()
{
    QFETCH(QString, zipName);
    QFETCH(QStringList, fileNames);
    QDir curDir;
    if (curDir.exists(zipName)) {
        if (!curDir.remove(zipName))
            QFAIL("Can't remove zip file");
    }
    if (!createTestFiles(fileNames)) {
        QFAIL("Can't create test files");
    }
    QVERIFY(JlCompress::compressDir(zipName, "tmp"));
    // get the file list and check it
    QStringList fileList = JlCompress::getFileList(zipName);
    qSort(fileList);
    qSort(fileNames);
    QCOMPARE(fileList, fileNames);
    removeTestFiles(fileNames);
    curDir.remove(zipName);
}

void TestJlCompress::extractFile_data()
{
    QTest::addColumn<QString>("zipName");
    QTest::addColumn<QStringList>("fileNames");
    QTest::addColumn<QString>("fileToExtract");
    QTest::addColumn<QString>("destName");
    QTest::newRow("simple") << "jlextfile.zip" << (
            QStringList() << "test0.txt" << "testdir1/test1.txt"
            << "testdir2/test2.txt" << "testdir2/subdir/test2sub.txt")
        << "testdir2/test2.txt" << "test2.txt";
}

void TestJlCompress::extractFile()
{
    QFETCH(QString, zipName);
    QFETCH(QStringList, fileNames);
    QFETCH(QString, fileToExtract);
    QFETCH(QString, destName);
    QDir curDir;
    if (!curDir.mkpath("jlext/jlfile")) {
        QFAIL("Couldn't mkpath jlext/jlfile");
    }
    if (!createTestFiles(fileNames)) {
        QFAIL("Couldn't create test files");
    }
    if (!JlCompress::compressDir(zipName, "tmp")) {
        QFAIL("Couldn't create test archive");
    }
    QVERIFY(!JlCompress::extractFile(zipName, fileToExtract,
                "jlext/jlfile/" + destName).isEmpty());
    QFileInfo destInfo("jlext/jlfile/" + destName), srcInfo("tmp/" +
            fileToExtract);
    QCOMPARE(destInfo.size(), srcInfo.size());
    curDir.remove("jlext/jlfile/" + destName);
    curDir.rmpath("jlext/jlfile");
    removeTestFiles(fileNames);
    curDir.remove(zipName);
}

void TestJlCompress::extractFiles_data()
{
    QTest::addColumn<QString>("zipName");
    QTest::addColumn<QStringList>("fileNames");
    QTest::addColumn<QStringList>("filesToExtract");
    QTest::newRow("simple") << "jlextfiles.zip" << (
            QStringList() << "test0.txt" << "testdir1/test1.txt"
            << "testdir2/test2.txt" << "testdir2/subdir/test2sub.txt")
        << (QStringList() << "testdir2/test2.txt" << "testdir1/test1.txt");
}

void TestJlCompress::extractFiles()
{
    QFETCH(QString, zipName);
    QFETCH(QStringList, fileNames);
    QFETCH(QStringList, filesToExtract);
    QDir curDir;
    if (!curDir.mkpath("jlext/jlfiles")) {
        QFAIL("Couldn't mkpath jlext/jlfiles");
    }
    if (!createTestFiles(fileNames)) {
        QFAIL("Couldn't create test files");
    }
    if (!JlCompress::compressDir(zipName, "tmp")) {
        QFAIL("Couldn't create test archive");
    }
    QVERIFY(!JlCompress::extractFiles(zipName, filesToExtract,
                "jlext/jlfiles").isEmpty());
    foreach (QString fileName, filesToExtract) {
        QFileInfo fileInfo("jlext/jlfiles/" + fileName);
        QCOMPARE(fileInfo.size(), QFileInfo("tmp/" + fileName).size());
        curDir.remove("jlext/jlfiles/" + fileName);
        curDir.rmpath(fileInfo.dir().path());
    }
    curDir.rmpath("jlext/jlfiles");
    removeTestFiles(fileNames);
    curDir.remove(zipName);
}

void TestJlCompress::extractDir_data()
{
    QTest::addColumn<QString>("zipName");
    QTest::addColumn<QStringList>("fileNames");
    QTest::newRow("simple") << "jlextdir.zip" << (
            QStringList() << "test0.txt" << "testdir1/test1.txt"
            << "testdir2/test2.txt" << "testdir2/subdir/test2sub.txt");
}

void TestJlCompress::extractDir()
{
    QFETCH(QString, zipName);
    QFETCH(QStringList, fileNames);
    QDir curDir;
    if (!curDir.mkpath("jlext/jldir")) {
        QFAIL("Couldn't mkpath jlext/jldir");
    }
    if (!createTestFiles(fileNames)) {
        QFAIL("Couldn't create test files");
    }
    if (!JlCompress::compressDir(zipName, "tmp")) {
        QFAIL("Couldn't create test archive");
    }
    QVERIFY(!JlCompress::extractDir(zipName, "jlext/jldir").isEmpty());
    foreach (QString fileName, fileNames) {
        QFileInfo fileInfo("jlext/jldir/" + fileName);
        QCOMPARE(fileInfo.size(), QFileInfo("tmp/" + fileName).size());
        curDir.remove("jlext/jldir/" + fileName);
        curDir.rmpath(fileInfo.dir().path());
    }
    curDir.rmpath("jlext/jldir");
    removeTestFiles(fileNames);
    curDir.remove(zipName);
}
