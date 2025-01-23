{atkmessages}
{if count($atkmessages)}
<div style='position:fixed;left:40%;top:150px;z-index:9999;margin-right:10px;' class="atkmessages">
  {foreach from=$atkmessages item=message}
    <div class="atkmessages">{$message.message}</div>
  {/foreach}
</div>
     <script>fade("atkmessages");</script>
{/if}

{foreach from=$blocks item=block}
  {$block}
{/foreach}
