    <div class="card card-secondary mb-4">

            {$formstart}

            {if isset($helplink)}<div class="card-header"><div align="right" class="col-md-12 helplink">{$helplink}</div></div>{/if}

            {atkmessages}
            {if count($atkmessages)}

                <div class="col-md-12"> 
                  
                        {foreach from=$atkmessages item=message}
                            <div  style="position:fixed;top:90px;" class="atkmessages">{$message.message}</div>
                        {/foreach}
                    
                    <script>fade("atkmessages");</script>
                </div>


            {/if}  

            {if (isset($header) && !empty($header))}
                <div class="card-header">{$header}</div>
            {/if}
            <div class="card-body">
			
            {$content}
            </div>
            <div class="card-footer">
                {foreach from=$buttons item=button}
                    &nbsp;{$button}&nbsp;
                {/foreach}</div>

            <!-- div box -->
            {$formend}
  
    </div>
