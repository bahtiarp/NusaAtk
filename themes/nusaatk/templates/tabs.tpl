<script language="JavaScript" type="text/javascript">
var tabs = new Array();
{section name=i loop=$tabs}
  tabs[tabs.length] = "{$tabs[i].tab}";
{/section}
</script>

        <ul class="nav nav-tabs">                              
          {section name=i loop=$tabs}
            <li id="tab_{$tabs[i].tab}"  class="{if $tabs[i].selected}active{/if}">	
              <a class="nav-link" data-toggle="tab" href="javascript:void(0)" onclick="showTab('{$tabs[i].tab}')">{$tabs[i].title}</a>
            </li>          
		      {/section}
        </ul>

<div class="tab-content">
            {$content}
</div>

