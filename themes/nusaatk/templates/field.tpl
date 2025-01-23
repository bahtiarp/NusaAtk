<div  {if $field.rowid != ""} id="{$field.rowid}"{/if}{if !$field.initial_on_tab} style="display: none"{/if} style="margin-bottom: 5px;" class="form-group row {$field.class}">
  {if isset($field.line)}
    <div>{$field.line}</div>
  {else}
    {if $field.label != "AF_NO_LABEL"}
      <label for="{$field.attribute}" class="col-sm-2 col-form-label {if isset($field.error)}errorlabel{else}{/if}">
        {if $field.label!=""}
          {$field.label}{if isset($field.obligatory)}{$field.obligatory}{/if}
        {/if}
      </label>
    {/if}
    <div id="{$field.id}"  {if $field.label=="AF_NO_LABEL"} class="col-sm-12" {else}class="col-sm-10"{/if} >
	
	
{if $field.label=="AF_NO_LABEL"} 
  	        <div id="{$field.id}"  class="row">
			  <div class="col-xs-12">
			  <div class="box">
		      {$field.full}
			  </div>
			  </div>
			</div> 
{else}

	        <div id="{$field.id}"  class="input-group input-group-sm" style="align-items: center;">
		      {$field.full}
			</div>
{/if}	
	
	
		
	

	
    </div>
  {/if}
</div>
