{if (count($errors)>0)}

        {$errortitle}<br/>
        {foreach from=$errors item=error}
          {$error.label}: {$error.message} {if $error.tablink} ({atktext id="error_tab"} {$error.tablink}){/if}<br/>
        {/foreach}

  {/if}

<div class="row">
<div class="col col-sm-6">
  {foreach from=$fields item=field}
    {if $field.column != 1}{include file="theme:field.tpl" field=$field}{/if}
  {/foreach}
</div>
<div class="col col-sm-6">
  {foreach from=$fields item=field}
    {if $field.column == 1}{include file="theme:field.tpl" field=$field}{/if}
  {/foreach}
</div>
</div>
