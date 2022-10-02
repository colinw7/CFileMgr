#include <CMime.h>
#include <COSUser.h>
#include <COSProcess.h>
#include <CStrUtil.h>
#include <CEnv.h>
#include <CFile.h>
#include <CConfig.h>
#include <CFileUtil.h>

bool
CMime::
getOpenFileCommand(const std::string &filename, std::string &cmd)
{
  return getFileCommand(filename, CFileUtil::getType(filename), "open", cmd);
}

bool
CMime::
getOpenFileCommand(const std::string &filename, CFileType type, std::string &cmd)
{
  return getFileCommand(filename, type, "open", cmd);
}

bool
CMime::
getFileCommand(const std::string &filename, CFileType type, const std::string &op, std::string &cmd)
{
  std::string appname;

  if (! CMimeInst->getApplication(type, op, appname))
    return false;

  cmd = CStrUtil::replace(appname, "%p", filename);

  return true;
}

bool
CMime::
getApplication(CFileType type, const std::string &name, std::string &appname)
{
  CConfig *config = getConfig();

  std::string type_str = CFileUtil::getTypeStr(type);

  return config->getValue(type_str + "/" + name, appname);
}

CConfig *
CMime::
getConfig()
{
  if (config_ == NULL)
    config_ = new CConfig("mime");

  return config_;
}

std::string
CMime::
getFileType(const std::string &filename)
{
  int                    pos2;
  std::string::size_type pos, pos1;

  std::string path = COSUser::getUserHome() + "/mime-magic.mime";

  if (CEnvInst.exists("CMIME_DIR"))
    path = CEnvInst.get("CMIME_DIR") + "/mime-magic.mime";

  std::string cmd;

  if (CFile::exists(path))
    cmd = "file -m " + path + " " + filename + " 2> /dev/null";
  else
    cmd = "file -i " + filename + " 2> /dev/null";

  std::string output;

  if (! COSProcess::executeCommand(cmd, output))
    goto fail;

  output = CStrUtil::stripSpaces(output);

  /* Get past the name specified */

  pos = output.find(':');

  if (pos == std::string::npos)
    goto fail;

  /* Get past ':' and any spaces */

  ++pos;

  pos2 = int(pos);

  CStrUtil::skipSpace(output, &pos2);

  pos = std::string::size_type(pos2);

  /* Nothing found OR not in MIME format of type/subtype */

  pos1 = output.find('/');

  if (pos == std::string::npos)
    goto fail;

  /* If a comma is found, only give portion before comma */

  pos1 = output.find(',');

  if (pos1 != std::string::npos)
    return output.substr(pos, pos1 - pos);

  /* if we are here, just return the string as is */

  return output.substr(pos);

 fail:
  return "application/unknown";
}
