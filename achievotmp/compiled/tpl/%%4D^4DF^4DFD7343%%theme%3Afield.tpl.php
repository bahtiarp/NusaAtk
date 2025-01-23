<?php /* Smarty version 2.6.31, created on 2024-11-14 05:15:01
         compiled from theme:field.tpl */ ?>
<div  <?php if ($this->_tpl_vars['field']['rowid'] != ""): ?> id="<?php echo $this->_tpl_vars['field']['rowid']; ?>
"<?php endif; ?><?php if (! $this->_tpl_vars['field']['initial_on_tab']): ?> style="display: none"<?php endif; ?> style="margin-bottom: 5px;" class="form-group row <?php echo $this->_tpl_vars['field']['class']; ?>
">
  <?php if (isset ( $this->_tpl_vars['field']['line'] )): ?>
    <div><?php echo $this->_tpl_vars['field']['line']; ?>
</div>
  <?php else: ?>
    <?php if ($this->_tpl_vars['field']['label'] != 'AF_NO_LABEL'): ?>
      <label for="<?php echo $this->_tpl_vars['field']['attribute']; ?>
" class="col-sm-2 col-form-label <?php if (isset ( $this->_tpl_vars['field']['error'] )): ?>errorlabel<?php else: ?><?php endif; ?>">
        <?php if ($this->_tpl_vars['field']['label'] != ""): ?>
          <?php echo $this->_tpl_vars['field']['label']; ?>
<?php if (isset ( $this->_tpl_vars['field']['obligatory'] )): ?><?php echo $this->_tpl_vars['field']['obligatory']; ?>
<?php endif; ?>
        <?php endif; ?>
      </label>
    <?php endif; ?>
    <div id="<?php echo $this->_tpl_vars['field']['id']; ?>
"  <?php if ($this->_tpl_vars['field']['label'] == 'AF_NO_LABEL'): ?> class="col-sm-12" <?php else: ?>class="col-sm-10"<?php endif; ?> >
	
	
<?php if ($this->_tpl_vars['field']['label'] == 'AF_NO_LABEL'): ?> 
  	        <div id="<?php echo $this->_tpl_vars['field']['id']; ?>
"  class="row">
			  <div class="col-xs-12">
			  <div class="box">
		      <?php echo $this->_tpl_vars['field']['full']; ?>

			  </div>
			  </div>
			</div> 
<?php else: ?>

	        <div id="<?php echo $this->_tpl_vars['field']['id']; ?>
"  class="input-group input-group-sm" style="align-items: center;">
		      <?php echo $this->_tpl_vars['field']['full']; ?>

			</div>
<?php endif; ?>	
	
	
		
	

	
    </div>
  <?php endif; ?>
</div>