{literal}
<style>

.clrboth {
    clear: both;
}

.pR10 {
    padding-right: 10px;
    margin-top: 10px;
    margin-bottom: 10px;
}

.tdout {
    background-color: #FFFFFF;
}

.iconlink {
  background: #F6F6F6 !important;
  border-bottom: 0 none;
}

.pL10 {
    padding-left: 10px;
}

.tmHeadBg, .tmHeadBg > a {
    background: none repeat scroll 0 0 #F6F6F6 !important;
    border-bottom: 0 solid #DFDFDF;
    border-top: 1px solid #F0F0F0;
    color: #888888 !important;
    font: 9px lucida Grande,Tahoma,Arial,Helvetica,sans-serif !important;
    height: 25px;
    text-shadow: 0 1px 0 #FFFFFF;
    text-transform: uppercase;
}

.tmHeadBg > a {
    border: 0 none !important;
}

.titleminmax {
    max-width: 1400px;
    white-space: normal;
}

.vat {
    vertical-align: top;
}
.pb5 {
    padding-bottom: 5px;
}
.pt3 {
    padding-top: 3px;
}
.lh18 {
    line-height: 18px;
}
.bdrbtm4 {
    border-bottom: 1px solid #F5F5F5;
}

.tdContainer {
    border-radius: 5px;
    -moz-border-radius: 5px;
    -webkit-border-radius: 5px;
    padding: 8px !important;

    background-color: #FFFFFF;
    border-color: #E5E5E5 #DBDBDB #D2D2D2;
    border-style: none;
    border-width: 1px;
    box-shadow: 0 1px 4px rgba(0, 0, 0, 0.3);
}

.recordList td {
    border-right: 0 none !important;
}

</style>
{/literal}

{if isset($formstart)}{$formstart}{/if}
<table cellspacing="0" cellpadding="0" class="recordListContainer pR10 clrboth">
  <tr>
    <td class="tdContainer">
        <table id="{$listid}" class="recordList" cellpadding="0" cellspacing="0">
        {if count($search)}
              <!-- search row -->
              <tr>
              {$searchstart}
              {foreach from=$search item=col}
                  <th class="recordListSearch" valign="{$vorientation}" {if isset($col.htmlattributes)}{$col.htmlattributes}{/if}>
                    {if $col.content != ""}{$col.content}{else}&nbsp;{/if}
                  </th>
              {/foreach}
              {$searchend}
              </tr>
            {/if}
            <!-- header -->
            <tr class="tdout">
              {section name=headerloop loop=$header}
                <th {if isset($header[headerloop].htmlattributes)}{$header[headerloop].htmlattributes}{/if}
                 {if $smarty.section.headerloop.index===0}class="recordListThFirst iconlink"{else}class="recordListTh tmHeadBg pL10 titleminmax"{/if}>
                  {if $header[headerloop].content != ""}{$header[headerloop].content}{else}&nbsp;{/if}
                </th>
              {/section}
            </tr>

            {if count($sort)}
              <!-- search row -->
              <tr>
              {$sortstart}
              {foreach from=$sort item=col}
                  <th valign="{$vorientation}" {if isset($col.htmlattributes)}{$col.htmlattributes}{/if}>
                    {if $col.content != ""}{$col.content}{else}&nbsp;{/if}
                  </th>
              {/foreach}
              {$sortend}
              </tr>
            {/if}

            <!-- records -->
            {$liststart}

            {foreach from=$rows item=row}
              <tr id="{$row.id}" class="row{if $row.rownum % 2 == 0 }1{else}2{/if}" {if $row.background!=""}style="background-color:{$row.background}" {/if}
                   onmouseover="highlightrow(this, '{$row.highlight}')"
                   onmouseout="resetrow(this)"
                   onclick="selectrow(this, '{$listid}', {$row.rownum})">
               {section name=colloop loop=$row.cols}
                <{if $row.type == "subtotal"}th{else}td {if $smarty.section.colloop.index===0}class="recordListTdFirst bdrbtm4"{else}class="recordListTd lh18 pt3 pb5 pL10 titleminmax bdrbtm4 vat"{/if}
                 {/if} valign="{$vorientation}" {if isset($row.cols[colloop].htmlattributes)}{$row.cols[colloop].htmlattributes}{/if}>
                  {if $row.cols[colloop].content != ""}{$row.cols[colloop].content}{else}&nbsp;{/if}
                </td>
              {/section}
            </tr>
            {/foreach}

            {if count($total)}
            <!-- totals row -->
              <tr>
              {foreach from=$total item=col}
                  <th valign="{$vorientation}" {if isset($col.htmlattributes)}{$col.htmlattributes}{/if}>
                    {if $col.content != ""}{$col.content}{else}&nbsp;{/if}
                  </th>
              {/foreach}
              </tr>
            {/if}
      </table>
    </td>
  </tr>
  {if $mra!=""}
  <!-- multirecord actions -->
   <tr>
     <td>
       <table border="0" cellspacing="0" cellpadding="0">
         <tr>
           <td valign="top"><img src="{$atkroot}atk/images/arrow.gif" border="0"></td>
           <td>
             {$mra} {$listend}
           </td>
         </tr>
       </table>
     </td>
   </tr>
  {/if}
</table>
{if isset($formend)}{$formend}{/if}