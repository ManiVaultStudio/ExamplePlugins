#include "Common.h"

QString getExampleNotificationMessage()
{
    return { "ManiVault Studio features a notification system that can be triggered from:<br>"
        "<table>"
        "<tr>"
        "<th style='text-align: left;'>Within a plugin:  </th>"
        "<td><a href='https://github.com/ManiVaultStudio/core/blob/3088ad09de6b5a5f3e84b24ffba44294bbb8001d/ManiVault/src/Plugin.h#L170'>addNotification(...)</a></td>"
        "</tr>"
        "<tr>"
        "<th style='text-align: left;'>The help manager:  </th>"
        "<td><a href='https://github.com/ManiVaultStudio/core/blob/126bd905d64d9b795dce188a06e9321eef132bd3/ManiVault/src/AbstractHelpManager.h#L100'>mv::help().addNotification(...)</a></td>"
        "</tr>"
        "</table>" };
}
