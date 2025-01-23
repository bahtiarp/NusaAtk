{if isset($formstart)}{$formstart}{/if}

      {if $mra!=""}
  <!-- multirecord actions -->

       {if $editing}
        {$mra}
       {else}

          <div class="d-flex flex-row" style="align-items: center;gap: 5px;margin-left: 25px;height: 30px;">
              <input type="checkbox" id="mracheck" value="" class="atkcheckbox" onclick="if(this.checked == true)
                  {literal}{{/literal}updateSelection('{$listid}', this.form, 'all');{literal}}{/literal}else{literal}{{/literal}updateSelection('{$listid}', this.form, 'none');{literal}}{/literal}">
                {$mra}
           </div>

       {/if}

	   
  {/if}

        <table id="{$listid}" class="table table-striped" cellpadding="0" cellspacing="0">
		<thead>
		 {if count($search)}
              <!-- search row -->
              <tr>
              {$searchstart}
              {foreach from=$search item=col}
                  <th class="recordListSearch" valign="{$vorientation}" {if isset($col.htmlattributes)}{$col.htmlattributes}{/if}>
                    {if  $col.content != ""}{$col.content}{else}&nbsp;{/if}
                  </th>
              {/foreach}
              {$searchend}
              </tr>
            {/if}
		
            <!-- header -->
            <tr>
              {section name=headerloop loop=$header}
                <th {if $header[headerloop].content == ""}{/if}
                    valign="{$vorientation}" {if isset($header[headerloop].htmlattributes)}{$header[headerloop].htmlattributes}{/if}
                 {if $smarty.section.headerloop.index===0}class="recordListThFirstXX"{else}class="recordListThXX"{/if}>
                  {if $header[headerloop].content != ""}{$header[headerloop].content}{else}&nbsp;{/if}
                </th>
              {/section}
            </tr>

            {if count($sort)}
              <!-- sort row -->
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
</thead>
           <tbody>

            <!-- records -->
            {$liststart}
            {foreach from=$rows item=row}
              <tr id="{$row.id}" class="row{if $row.rownum % 2 == 0 }1{else}2{/if}" {if $row.background!=""}style="background-color:{$row.background}" {/if}
                   onmouseover="highlightrow(this, '{$row.highlight}')"
                   onmouseout="resetrow(this)"
                   onclick="selectrow(this, '{$listid}', {$row.rownum})">
               {section name=colloop loop=$row.cols}
               <{if $row.type == "subtotal"}th{else}td{/if}
                    class="{if $smarty.section.colloop.index===0}recordListTdFirst{else}recordListTd{/if}{if $row.cols[colloop].type == "data"} clickable{/if}"
                    valign="{$vorientation}"  {if isset($row.cols[colloop].htmlattributes)}{$row.cols[colloop].htmlattributes}{/if}
                    {if $row.cols[colloop].type == "data"} {/if}
                    {if $row.cols[colloop].type != "data"}style="width:1px;background-color: #f5f5f5"{/if}
                    >
                  {if $row.cols[colloop].content != ""}{$row.cols[colloop].content}{else}&nbsp;{/if}
                </{if $row.type == "subtotal"}th{else}td{/if}>
              {/section}
            </tr>
            {/foreach}
            {$listend}

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
			</tbody>
      </table>
{if isset($formend)}{$formend}{/if}
