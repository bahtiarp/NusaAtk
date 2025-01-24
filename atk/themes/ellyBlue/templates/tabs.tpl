<script language="JavaScript" type="text/javascript">
    var tabs = new Array();
{section name=i loop=$tabs}tabs[tabs.length] = "{$tabs[i].tab}"; {/section}

    var tabLeftImage = "{atkthemeimg tab_left.png}";
    var tabRightImage = "{atkthemeimg tab_right.png}";
    var tabBackgroundImage = "{atkthemeimg tab_back.png}";
    var tabSelectedLeftImage = "{atkthemeimg tab_left_s.png}";
    var tabSelectedRightImage = "{atkthemeimg tab_right_s.png}";
    var tabSelectedBackgroundImage = "{atkthemeimg tab_back_s.png}";

    var tabColor = "#FFFFFF";
    var tabSelectedColor = "#000000";
</script>

{literal}
<style>

.tabOuterDiv {
    float: left;
    max-width: 200px;
    position: relative;
    width: 200px;
}

#tabContainer {
    padding: 0 !important;
}

#tabContent {
    margin-left: 200px;
    min-width: 500px;
    padding-top: 0 !important;
}

.tabInnerDiv {
    margin-right: 0 !important;
}

.tabOuterDiv {
    border-bottom: 0;
}

.tabOn, .tabOff {
    line-height: 30px;
    border-top-left-radius: 0;
    border-top-right-radius: 0;
    text-align: left;
    width: 176px !important;
    color: #333333 !important;
    margin-bottom: 0;
}

.tabOff {
    background: none;
    border-bottom: 1px dotted #9AC841;
}

/*.tabOff:hover {
    color: #FFFFFF !important;
}*/

</style>
{/literal}
<div class="tabOuterDiv">
    <div id="tabContainer">
        {section name=i loop=$tabs}
            <div class="tabInnerDiv">
                <div id="tab_{$tabs[i].tab}" style="position: absolute;">
                    <div onclick="showTab('{$tabs[i].tab}')">
                        <div class="tabOn">{$tabs[i].title}</div>
                    </div>
                </div>
                <div onclick="showTab('{$tabs[i].tab}')">
                    <div class="tabOff">{$tabs[i].title}</div>
                </div>
            </div>
        {/section}
    </div>

    <!-- <table border="0" cellpadding="0" cellspacing="0" id="tabContainer">
            {section name=i loop=$tabs}
        <tr>
                <td valign="bottom">
                    <div class="tabInnerDiv">
                        <div id="tab_{$tabs[i].tab}" style="position: absolute;">
                            <table border="0" cellspacing="0" cellpadding="0">
                                <tr onclick="showTab('{$tabs[i].tab}')">
                                    <td height="22" valign="middle" align="center" nowrap class="tabOn">
                                        <span style="color: #ff0000;">{$tabs[i].title}</span>
                                    </td>
                                </tr>
                            </table>
                        </div>
                        <table border="0" cellspacing="0" cellpadding="0" style="cursor: pointer;">
                            <tr onclick="showTab('{$tabs[i].tab}')">
                                <td height="22" valign="middle" align="center" nowrap class="tabOff">
                                    {$tabs[i].title}
                                </td>
                            </tr>
                        </table>
                    </div>
                </td>
        </tr>
            {/section}
    </table> -->
</div>

<div id="tabContent">
    {$content}
</div>