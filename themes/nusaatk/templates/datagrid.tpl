{assign var=top value=null}
{assign var=editcontrol value=null}
{assign var=bottom value=null}
{if $index || $editcontrol}
  <div class="card-header">

      {if $editcontrol}{$editcontrol}{/if} {if $index}{$index}{/if}

  </div>
{elseif $paginator || $limit}
  <div class="card-footer">
    <div class="col-sm-12">
      {if $editcontrol}{$editcontrol}{/if} {if $paginator}{$paginator}{/if}
    </div>
    <div class="col-sm-12">
      {if $limit}{$limit}{/if}
    </div>
  </div>
{/if}

 <div class="card-body p-0" style="width:100%;max-height:68vh;overflow-y:auto;">
  {$list}
</div>
<div class="card-footer">

{if $bottom}
  <div class="float-start">
    {$bottom}
  </div>
{/if}

{if $paginator || $summary}
    
      {if $paginator}<div class="float-start">{$paginator}</div>{/if}
      {if $summary}<div class="float-end">{$summary}</div>{/if}

 
{/if}
</div>
