<?php /* Smarty version 2.6.31, created on 2024-10-23 14:18:03
         compiled from ./modules/Receipt_Management/tpl/result.tpl */ ?>
<div class="row justify-content-center">
    <div class="col-6">
        <div class="card">
            <h5 class="card-header">Detail Pengiriman</h5>
            <div class="card-body">
              
              <p class="card-text"><b>Asal Muat:</b> <?php echo $this->_tpl_vars['data']['port_of_loading']['name']; ?>
 </p>
              <p class="card-text"><b>Tujuan:</b> <?php echo $this->_tpl_vars['data']['port_of_discharge']['name']; ?>
 </p>
              <p class="card-text"><b>Nama Pengirim:</b> <?php echo $this->_tpl_vars['data']['shipper']['name']; ?>
 </p>
              <p class="card-text"><b>Nama Penerima:</b> <?php echo $this->_tpl_vars['data']['consignee']['name']; ?>
 </p>
              <p class="card-text"><b>Tanggal Stuffing:</b> <?php echo $this->_tpl_vars['data']['stuffing_date']['day']; ?>
 <?php echo $this->_tpl_vars['data']['stuffing_date']['month']; ?>
 <?php echo $this->_tpl_vars['data']['stuffing_date']['year']; ?>
 </p>
              <p class="card-text"><b>ETD:</b> <?php echo $this->_tpl_vars['data']['etd']['day']; ?>
 <?php echo $this->_tpl_vars['data']['etd']['month']; ?>
 <?php echo $this->_tpl_vars['data']['etd']['year']; ?>
  </p>
              <p class="card-text"><b>ETA:</b> <?php echo $this->_tpl_vars['data']['eta']['day']; ?>
 <?php echo $this->_tpl_vars['data']['eta']['month']; ?>
 <?php echo $this->_tpl_vars['data']['eta']['year']; ?>
</p>
              <p class="card-text"><b>Nama Kapal:</b> <?php echo $this->_tpl_vars['data']['vessel']['name']; ?>
  </p>
              <p class="card-text"><b>Type Container:</b> <?php echo $this->_tpl_vars['data']['container_type']['type']; ?>
 </p>
              <p class="card-text"><b>Status:</b> <?php echo $this->_tpl_vars['data']['status']['status']; ?>
</p>
            </div>
          </div>
    </div>
</div>