#ifndef HTMLCONTENTTYPE_HPP
#define HTMLCONTENTTYPE_HPP

#include <string>

enum class HttpCharSet : int
{
    ISO88591,
    ISO88598,
    UTF8,
    UTF16,
    Unknown
};

enum class HttpContentType : int
{
    ApplicationEDIX12,
    ApplicationEDIFACT,
    ApplicationJavaScript,
    ApplicationOctetStream,
    ApplicationOgg,
    ApplicationPdf,
    ApplicationXhtmlXml,
    ApplicationXShockWaveFlash,
    ApplicationJson,
    ApplicationLdJson,
    ApplicationXml,
    ApplicationZip,
    ApplicationXWWWFormUrlEncoded,
    AudioMpeg,
    AudioXMSWma,
    AudioVndRnRealAudio,
    AudioXWav,
    ImageGif,
    ImageJpeg,
    ImagePng,
    ImageTiff,
    ImageVndMicrosoftIcon,
    ImageXIcon,
    ImageVndDjvu,
    ImageSvgXml,
    MultiPartMixed,
    MultiPartAlternative,
    MultiPartRelated,
    MultiPartFormData,
    TextCss,
    TextCsv,
    TextHtml,
    TextJavaScript,
    TextPlain,
    TextXml,
    VideoMpeg,
    VideoMp4,
    VideoQuickTime,
    VideoXMSWmv,
    VideoXMSVideo,
    VideoXFlv,
    VideoWebM,
    ApplicationVndOasisOpenDocumentText,
    ApplicationVndOasisOpenDocumentSpreadSheet,
    ApplicationVndOasisOpenDocumentPresentation,
    ApplicationVndOasisOpenDocumentGraphics,
    ApplicationVndMSExcel,
    ApplicationVndOpenXmlFormatsOfficeDocumentSpreadSheetMLSheet,
    ApplicationVndMSPowerpoint,
    ApplicationVndOpenXmlFormatsOfficeDocumentPresentationMLPresentation,
    ApplicationMSWord,
    ApplicationVndOpenXmlFormatsOfficeDocumentWordProcessingMLDocument,
    ApplicationVndMozillaXulXml,
    Unknown,
    Any = Unknown
};

class HttpContent
{
public:
    static HttpContentType GetTypeFromFileExtension(const std::string &filePath);
    static void GetTypeFromHeader(const std::string &header, HttpContentType &contentType, HttpCharSet &charSet);
    static std::string GetStringFromContentType(HttpContentType contentType);
    static bool TryGetMimeType(const std::string &fileNameOrExtension, std::string &mimeType);
};

#endif