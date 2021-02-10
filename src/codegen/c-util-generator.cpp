#include "c-util-generator.h"
#include "helpers/formatter.h"
#include <algorithm>

static const std::string openguard = "#ifdef __cplusplus\n\
extern \"C\" {\n\
#endif";

static const std::string closeguard = "#ifdef __cplusplus\n\
}\n\
#endif";

CiUtilGenerator::CiUtilGenerator()
{
  Clear();
  tof = new FileWriter;
}

void CiUtilGenerator::Clear()
{
  // clear all groups of messages
  tx.clear();
  rx.clear();
  both.clear();
}


void CiUtilGenerator::Generate(std::vector<MessageDescriptor_t*>& msgs, const FsDescriptor_t& fsd,
  const MsgsClassification& groups, const std::string& drvname)
{
  Clear();

  code_drvname = drvname;
  file_drvname = str_tolower(drvname);

  // 1 step is to prepare vectors of message's groups
  for (auto m : msgs)
  {
    auto v = std::find_if(groups.Both.begin(), groups.Both.end(), [&](const uint32_t& msgid)
    {
      return msgid == m->MsgID;
    });

    if (v != std::end(groups.Both))
    {
      // message is in Both group, so put it to rx and tx containers
      tx.push_back(m);
      rx.push_back(m);
      continue;
    }

    v = std::find_if(groups.Rx.begin(), groups.Rx.end(), [&](const uint32_t& msgid)
    {
      return msgid == m->MsgID;
    });

    if (v != std::end(groups.Rx))
    {
      rx.push_back(m);
      continue;
    }

    v = std::find_if(groups.Tx.begin(), groups.Tx.end(), [&](const uint32_t& msgid)
    {
      return msgid == m->MsgID;
    });

    if (v != std::end(groups.Tx))
    {
      tx.push_back(m);
      continue;
    }
  }

  std::sort(rx.begin(), rx.end(), [&](const MessageDescriptor_t* a, const MessageDescriptor_t* b)
  {
    return a->MsgID < b->MsgID;
  });

  std::sort(tx.begin(), tx.end(), [&](const MessageDescriptor_t* a, const MessageDescriptor_t* b)
  {
    return a->MsgID < b->MsgID;
  });

  fdesc = &fsd;

  PrintHeader();

  PrintSource();

}

void CiUtilGenerator::PrintHeader()
{
  tof->Flush();

  tof->AppendLine("#pragma once", 2);

  tof->AppendLine(openguard.c_str(), 2);

  // include common dbc code config header
  tof->AppendLine("#include \"dbccodeconf.h\"", 2);
  // include c-main driver header
  tof->AppendLine(StrPrint("#include \"%s.h\"", file_drvname.c_str()), 2);

  if (rx.size() == 0)
  {
    tof->AppendLine("// There is no any RX mapped massage.", 2);
  }
  else
  {
    // print the typedef
    tof->AppendLine("typedef struct\n{");

    for (auto m : rx)
    {
      tof->AppendLine(StrPrint("  %s_t %s;", m->Name.c_str(), m->Name.c_str()));
    }

    tof->AppendLine(StrPrint("} %s_rx_t;", fdesc->drvname.c_str()), 2);
  }

  if (tx.size() == 0)
  {
    tof->AppendLine("// There is no any TX mapped massage.", 2);
  }
  else
  {
    // print the typedef
    tof->AppendLine("typedef struct\n{");

    for (auto m : tx)
    {
      tof->AppendLine(StrPrint("  %s_t %s;", m->Name.c_str(), m->Name.c_str()));
    }

    tof->AppendLine(StrPrint("} %s_tx_t;", fdesc->drvname.c_str()), 2);
  }

  if (rx.size() > 0)
  {
    // receive function necessary only when more than 0 rx messages were mapped
    tof->AppendLine(StrPrint("uint32_t %s_Receive(%s_rx_t* m, const uint8_t* d, uint32_t msgid, uint8_t dlc);",
        fdesc->drvname.c_str(), fdesc->drvname.c_str()), 2);
  }

  // print extern for super structs
  if (rx.size() > 0 || tx.size() > 0)
  {
    tof->AppendLine(StrPrint("#ifdef __DEF_%s__", fdesc->DRVNAME.c_str()), 2);

    if (rx.size() > 0)
    {
      tof->AppendLine(StrPrint("extern %s_rx_t %s_rx;", fdesc->drvname.c_str(), fdesc->drvname.c_str()), 2);
    }

    if (tx.size() > 0)
    {
      tof->AppendLine(StrPrint("extern %s_tx_t %s_tx;", fdesc->drvname.c_str(), fdesc->drvname.c_str()), 2);
    }

    tof->AppendLine(StrPrint("#endif // __DEF_%s__", fdesc->DRVNAME.c_str()), 2);
  }

  tof->AppendLine(closeguard.c_str());

  tof->Flush(fdesc->util_h.fpath);
}

void CiUtilGenerator::PrintSource()
{
}
