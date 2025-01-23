<?php /* Smarty version 2.6.31, created on 2024-10-23 11:54:44
         compiled from ./themes/nusaatk/templates/datagrid.tpl */ ?>
<?php $this->assign('top', null); ?>
<?php $this->assign('editcontrol', null); ?>
<?php $this->assign('bottom', null); ?>
<?php if ($this->_tpl_vars['index'] || $this->_tpl_vars['editcontrol']): ?>
  <div class="card-header">

      <?php if ($this->_tpl_vars['editcontrol']): ?><?php echo $this->_tpl_vars['editcontrol']; ?>
<?php endif; ?> <?php if ($this->_tpl_vars['index']): ?><?php echo $this->_tpl_vars['index']; ?>
<?php endif; ?>

  </div>
<?php elseif ($this->_tpl_vars['paginator'] || $this->_tpl_vars['limit']): ?>
  <div class="card-footer">
    <div class="col-sm-12">
      <?php if ($this->_tpl_vars['editcontrol']): ?><?php echo $this->_tpl_vars['editcontrol']; ?>
<?php endif; ?> <?php if ($this->_tpl_vars['paginator']): ?><?php echo $this->_tpl_vars['paginator']; ?>
<?php endif; ?>
    </div>
    <div class="col-sm-12">
      <?php if ($this->_tpl_vars['limit']): ?><?php echo $this->_tpl_vars['limit']; ?>
<?php endif; ?>
    </div>
  </div>
<?php endif; ?>

 <div class="card-body p-0" style="width:100%;max-height:68vh;overflow-y:auto;">
  <?php echo $this->_tpl_vars['list']; ?>

</div>
<div class="card-footer">

<?php if ($this->_tpl_vars['bottom']): ?>
  <div class="float-start">
    <?php echo $this->_tpl_vars['bottom']; ?>

  </div>
<?php endif; ?>

<?php if ($this->_tpl_vars['paginator'] || $this->_tpl_vars['summary']): ?>
    
      <?php if ($this->_tpl_vars['paginator']): ?><div class="float-start"><?php echo $this->_tpl_vars['paginator']; ?>
</div><?php endif; ?>
      <?php if ($this->_tpl_vars['summary']): ?><div class="float-end"><?php echo $this->_tpl_vars['summary']; ?>
</div><?php endif; ?>

 
<?php endif; ?>
</div>