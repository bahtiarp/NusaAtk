{if $links}
<ul class="pagination">
{/if}
{foreach from=$links item='link' key='i'}
  {if $link.current}
    <li class="active"><a>{$link.title|escape} <span class="sr-only">(current)</span></a></li>
  {else}
  		{if $link.title == 'Previous'}
	  	<li><a href="javascript:void(0)" onclick="{$link.call|escape}" title="{$link.title|escape}">&laquo;</a></li>
	  	{elseif $link.title == 'Next'}
	  	<li><a href="javascript:void(0)" onclick="{$link.call|escape}" title="{$link.title|escape}">&raquo;</a></li>
	  	{else}
	  	<li><a href="javascript:void(0)" onclick="{$link.call|escape}" title="{$link.title|escape}">{$link.title|escape}</a></li>
	  {/if}
  {/if}
{/foreach}
{if $links}
</ul>
{/if}
