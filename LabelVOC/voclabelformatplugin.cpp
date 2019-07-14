#include "voclabelformatplugin.h"

bool VOCLabelFormatPlugin::save(const BoxManager &boxes, const QString &savePath)
{

}

QString VOCLabelFormatPlugin::formatDescription()
{
    return "VOC format for annotation";
}

QString VOCLabelFormatPlugin::formatExtension()
{
    return "*.xml";
}

QString VOCLabelFormatPlugin::formatName()
{
    return "VOC XML";
}
