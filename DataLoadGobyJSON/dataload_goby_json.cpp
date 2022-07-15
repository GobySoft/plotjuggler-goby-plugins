#include "dataload_goby_json.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QProgressDialog>
#include <QDateTime>
#include <QInputDialog>

#include "nlohmann_parsers.h"


DataLoadGobyJSON::DataLoadGobyJSON()
{
  _extensions.push_back("json");
}

const QRegExp csv_separator("(\\,)");

const std::vector<const char*>& DataLoadGobyJSON::compatibleFileExtensions() const
{
  return _extensions;
}

bool DataLoadGobyJSON::readDataFromFile(FileLoadInfo* info, PlotDataMapRef& plot_data)
{

    // group + type
    std::map<std::string, std::unique_ptr<JSON_Parser>> parsers_;
    
    QFile file(info->filename);
    if( !file.open(QFile::ReadOnly) )
    {
        return false;
    }
    QTextStream text_stream(&file);

    //-----------------
    // read the file line by line
    int linecount = 1;
    while (!text_stream.atEnd())
    {
        std::string line = text_stream.readLine().toStdString();
        //        std::cout << line << std::endl;
        auto j = nlohmann::json::parse(line);
        std::string topic = j["_group_"].get<std::string>() + "__" + j["_type_"].get<std::string>();
        std::uint64_t utime = j["_utime_"].get<std::uint64_t>();
        double timestamp = static_cast<double>(utime) / 1.0e6;
        
        if(!parsers_.count(topic))
            parsers_[topic] = std::make_unique<JSON_Parser>(topic, plot_data, false, "_dtime_");
        
        std::vector< std:: uint8_t > data(line.begin(), line.end());
        MessageRef msg_serialized(data);
        parsers_[topic]->parseMessage(msg_serialized, timestamp);
        linecount++;
    }

  file.close();

  return true;
}






