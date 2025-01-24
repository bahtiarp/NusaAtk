{foreach from=$links item='link' key='i'}
  {if $i > 0}
    |
  {/if}
  
  {if $link.current}
    <span class="bagde badge-danger">{$link.title}</span>
  {else}
    <span href="javascript:void(0)" style="cursor: pointer;" class="badge text-bg-primary" onclick="{$link.call|escape}" title="{$link.title|escape}">{$link.title|escape}</span>
  {/if}
{/foreach}
