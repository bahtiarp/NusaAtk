{if isset($formstart)}{$formstart}{/if}

  {atkmessages}
  {if count($atkmessages)}

 
           <div style='position:fixed;left:40%;top:150px;z-index:9999;margin-right:10px;' class="atkmessages">
          {foreach from=$atkmessages item=message}
            <div class="atkmessages">{$message.message}</div>
          {/foreach}             
    
        </div>
        <script>fade("atkmessages");</script>
      
  {/if}  
  {if (isset($header) && !empty($header))}
<div class="list-header">
   {$header}
  </div>
  {/if}
  <!--
  {if (isset($index) && !empty($index))}
  <div>
    <span valign="top" align="left">{$index}<br><br></span>
  </div>
  {/if}
  -->
  {if (isset($navbar) && !empty($navbar))}
  <div>
    <span valign="top" align="left">{$navbar}<br></span>
  </div>
  {/if}
  <div class="noname">
    <span valign="top" align="left">
        {$list}
    </span>
  </div>
  {if (isset($navbar) && !empty($navbar))}
  <div>
    <span valign="top" align="left">{$navbar}<br></span>
  </div>
  {/if}
  {if (isset($footer) && !empty($footer))}
  <div>
    <span valign="top" align="left">{$footer}<br></span>
  </div>
  {/if}

{if isset($formstart)}{$formend}{/if}