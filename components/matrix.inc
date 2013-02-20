<?php

/**
 * @file
 * Webform module matrix component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_matrix() {
    return array(
        'name' => '',
        'form_key' => NULL,
        'pid' => 0,
        'weight' => 0,
        'value' => '',
        'mandatory' => 0,
        'extra' => array(
            'matrix_col' => '1',
            'matrix_row' => '1',
            'title_display' => 0,
            'description' => '',
            'private' => FALSE,
        ),
    );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_matrix() {
     return array(
      'webform_display_matrix' => array(
      'render element' => 'element',
      'file' => 'components/matrix.inc',
      'path'=>  drupal_get_path('module', 'webform_matrix_component'),
      ),
      ); 
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_matrix($component) {
    
    $form=array();
     $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Label'),
        '#default_value' => $component['name'],
        '#description' => t('The default value of the field.') . theme('webform_token_help'),
        '#size' => 60,
        '#maxlength' => 127,
        '#weight' => 0,
        '#required'=>TRUE,
    );

	$max_rows=variable_get('webform_matrix_rows',10);
	$row_options=array();
	for($row=1;$row<=$max_rows;$row++){
		$row_options[$row]=$row;
	}

    $form['extra']['matrix_row'] = array(
        '#type' => 'select',
        '#title' => t('Matrix Rows'),
		'#options' => $row_options,
        '#default_value' => isset($component['extra']['matrix_row']) ? $component['extra']['matrix_row'] : "1",
        '#weight' => 0,
        '#parents'=>array('extra','matrix_row'),

    );

    return $form;
}

function webform_matrix_get_column_form($form,$form_state,$component){

	$max_cols=variable_get('webform_matrix_cols',10);
	$col_options=array();
	for($col=1;$col<=$max_cols;$col++){
		$col_options[$col]=$col;
	}
    $form['extra']['matrix_col'] = array(
        '#type' => 'select',
        '#title' => t('Matrix Column'),
        '#options' => $col_options, 
        '#default_value' => isset($component['extra']['matrix_col']) ? $component['extra']['matrix_col'] : "",
       // '#attributes' => array('onchange' => 'get_element_list(); return false;'),
        '#description' => t('Choose this will remove all your previos settings'),
        '#parents'=>array('extra','matrix_col'),
        '#ajax'=>array(
            'wrapper'=>'matrix-settings-ajax',
            'callback' => '_matrix_get_type_option',
        ),
    );
    
    $form['extra']['element'] = array(
        '#type' => 'item',
      //  '#markup' => $html,
        '#title' => t('Configuration'),
        '#prefix' => '<div id="matrix-settings-ajax">',
        '#suffix' => '</div>',
        '#tree' => TRUE,
    );
    
    $input_component=!empty($form_state['input'])?$form_state['input']['extra']:$component['extra'];
    $matrix_col=isset($input_component['matrix_col'])?$input_component['matrix_col']:1;
    $ele_options = array('textfield' => 'Textfield', 'select' => 'Select', 'label' => 'Label', 'date' => 'Date');
    //$html = "<table><tbody><tr>";
    
    for($i=1;$i<=$matrix_col;$i++){
        $element_type='textfield';
        $element_id=$element_title='element-'.$i;
        $option_array="";
        $label_name="";
        if(isset($input_component['element'][$element_id])){
            $elementvalue=$input_component['element'][$element_id];
            $element_type = @$elementvalue['type'];
            $element_title=@$elementvalue['title'];
            $option_array=@$elementvalue['option'];
            $label_name=@$elementvalue['label_name'];
            
        }
        
        $td="<td id='$element_id'>";
        $prefix=($i==1)?"<table id='matrix-options'><tbody><tr>".$td:$td;
        $suffix=($matrix_col==$i)?"</td></tr></tbody></table>":"</td>";
        $form['extra']['element'][$element_id]['type']=array(
          '#type'=>'select',
          '#title'=>$element_id,  
          '#options'=>$ele_options,
          '#default_value'=>$element_type,
          '#attributes'=>array('onchange'=>"show_hide_element_type('$element_id');"),  
          '#prefix'=>$prefix,  
          '#parents'=>array('extra','element',$element_id,'type'),
          '#id'=>"edit-extra-element-$element_id-type", 
        //  '#suffix'=>$suffix,  
        );
        $form['extra']['element'][$element_id]['title']=array(
            '#type'=>'textfield',
            '#title'=>'Title',
            '#size'=>15,
            '#default_value'=>$element_title,
            '#parents'=>array('extra','element',$element_id,'title'),
            '#id'=>"edit-extra-element-$element_id-title", 
			'#required'=>TRUE,
          //  '#prefix'=>$prefix,  
            //'#suffix'=>$suffix,  
        );
        $option_div_display=($element_type=='select')?'block':'none';
		
        $form['extra']['element'][$element_id]['option']=array(
            '#type'=>'textarea',
            '#title'=>'Option',
            '#rows'=>2,
            '#cols'=>15,
            '#default_value'=>$option_array,
            '#parents'=>array('extra','element',$element_id,'option'),
            '#id'=>"edit-extra-element-$element_id-option", 
            '#prefix'=>"<div class='parent-form-item-extra-element-$element_id-option' style='display:$option_div_display'>",  
            '#suffix'=>"</div>",  
			'#description'=>t('<strong>Key-value pairs MUST be specified as "safe_key|Some readable option"</strong>. Use of only alphanumeric characters and underscores is recommended in keys.'),
			
        );
        $label_div_display=($element_type=='label')?'block':'none';
        $form['extra']['element'][$element_id]['label_name']=array(
            '#type'=>'textfield',
            '#title'=>'Label Name',
            '#default_value'=>$label_name,
            '#size'=>15,
            '#id'=>"edit-extra-element-$element_id-label_name", 
            '#prefix'=>"<div class='parent-form-item-extra-element-$element_id-label-name' style='display:$label_div_display'>",  
            '#suffix'=>"</div>".$suffix,  
            '#parents'=>array('extra','element',$element_id,'label_name'),
        );
        
       
    }
    $form['#validate'][]='_webform_edit_validate_matrix';
    //$form['#submit'][] = 'webform_wrapper_matrix_extra_property';
    //$form['#submit'][]='webform_component_edit_form_submit';
    return $form;
}

//function webform-component-edit-form
function _matrix_get_type_option(&$form,&$form_state){
    
    return $form['extra']['element'];
}



/**
 * Implements _webform_render_component().
 */
function _webform_render_matrix($component, $value = NULL, $filter = TRUE) {
	drupal_add_css(drupal_get_path('module','webform_matrix_component').'/webform_matrix_component.css');	

    $nid=$component['nid'];
    $cid=$component['cid'];
    $element=array();
    
    $element['value'] = array(
        '#markup' => "<label>".$component['name']."</label>",
    );
	$datepicker=false;
    if (isset($component['extra']['matrix_col'])) {
        $editvalue = unserialize($value[0]);
        $matrix_col = $component['extra']['matrix_col'];
        $matrix_row = $component['extra']['matrix_row'];
        for ($prow = 1; $prow <= $matrix_row; $prow++) {
            if (isset($component['extra']['element'])) {
                foreach ($component['extra']['element'] as $elekey => $elevalue) {
                    list($j, $i) = explode('-', $elekey);
                    //for ($i=1;$i<=$matrix_col;$i++){
                    //$type=$component['extra']['col'.$i];
                    $prefix =($i == 1 && $prow==1) ? "<div class='matrix-render-div'>" : "";
                    $suffix =($i == $matrix_col && $prow==$matrix_row) ? "</div>" : "";
                    
		    $prefix .= ($i == 1) ? "<div class='matrix-render-row-div matrix-render-col-$matrix_col'>" : "";
                    $suffix .= ($i == $matrix_col) ? "</div>" : "";
		    $width=100/$matrix_col;			
                    $width=$width-.1;
                    $prefix .="<div style='width:$width% !important;display:block !important;float:left !important'>";
                    $suffix .="</div>";
                    $eletype = $elevalue['type'];
                    $title = isset($elevalue['title'])?$elevalue['title']:$elekey;
                    $title=($prow==1)?$title:"";
                    switch ($eletype) {
                        case 'textfield':
                            //$title=$component['extra']['preview']['col'.$i]['title'];
                            
                            $element[$prow][$i] = array(
                                '#type' => 'textfield',
                                '#title' => $title,
                                '#prefix' => $prefix,
                                '#suffix' => $suffix,
                                '#maxlength' => 50,
								'#default_value' => $editvalue[$prow][$i],
                            );
                            break;
                        case 'select':
                            $test_option = array(1, 2, 3, 4);
                            //$options=$component['extra']['preview']['col'.$i]['option'];
                            $options = $elevalue['option'];
                            $option_array = array();
                            
							$option_array=matrix_select_option_from_text($options);
                            $element[$prow][$i] = array(
                                '#type' => 'select',
                                '#title' => $title,
                                '#options' => $option_array,
                                '#prefix' => $prefix,
                                '#suffix' => $suffix,
                                '#default_value' => $editvalue[$prow][$i],
                            );
                            break;
                        case 'date':
                            $element[$prow][$i] = array(
                                '#type' => 'date',
                                '#title' => $title,
                                '#prefix' => $prefix."<div class='date_matrix'>",
                                '#suffix' => "</div>".$suffix,
                                '#default_value' => $editvalue[$prow][$i],
                                    //'#options'=>$test_option,  
                            );
                            break;
                        case 'label':
                            //$label_name=$component['extra']['preview']['col'.$i]['label_name'];
                            $label_name = $elevalue['label_name'];
                            $element[$prow][$i] = array(
                                '#type' => 'item',
                                '#title' => $title,
                                '#markup' => "<div class='form-item'>".$label_name."</div>",
                                '#prefix' => $prefix,
                                '#suffix' => $suffix,
                                '#default_value' => $editvalue[$prow][$i],
                            );
                            break;
                    }
                }
            }
        }
    }
	
    return $element;
}

function _webform_edit_validate_matrix($form, &$form_state) {
	$subelements=$form_state['values']['extra']['element'];
	foreach($subelements as $subkey => $subelement){
		if($subelement['type']=='select'){
			if(empty($subelement['label_name'])){
				form_error($form['extra']['element'][$subkey]['label_name'],t('Label Name field is required.'));
			}
		}
		if($subelement['type']=='select'){
			if(empty($subelement['option'])){
				form_error($form['extra']['element'][$subkey]['option'],t('Options field is required.'));
			}else{
				$lines = explode("\n", trim($subelement['option']));
				$existing_keys = array();
				$duplicate_keys = array();
				$missing_keys = array();
				$long_keys = array();
				$group = '';
				foreach ($lines as $line) {
				  $matches = array();
				  $line = trim($line);
				  if (preg_match('/^\<([^>]*)\>$/', $line, $matches)) {
					$group = $matches[1];
					$key = NULL; // No need to store group names.
				  }
				  elseif (preg_match('/^([^|]*)\|(.*)$/', $line, $matches)) {
					$key = $matches[1];
					if (strlen($key) > 128) {
					  $long_keys[] = $key;
					}
				  }
				  else {
					$missing_keys[] = $line;
				  }

				  if (isset($key)) {
					if (isset($existing_keys[$group][$key])) {
					  $duplicate_keys[$key] = $key;
					}
					else {
					  $existing_keys[$group][$key] = $key;
					}
				  }
				}
				
				if (!empty($missing_keys)) {
				  form_error($form['extra']['element'][$subkey]['option'], t('Every option must have a key specified. Specify each option as "safe_key|Some readable option".'));
				}

				if (!empty($long_keys)) {
				  form_error($form['extra']['element'][$key]['option'], t('Option keys must be less than 128 characters. The following keys exceed this limit:') . theme('item_list', $long_keys));
				}
				
				if (!empty($duplicate_keys)) {
				  form_error($form['extra']['element'][$subkey]['option'], t('Options within the select list must be unique. The following keys have been used multiple times:'). theme('item_list', array('items' => $duplicate_keys)));
				}
			
			}
		}
	}
}

/**
 * Implements _webform_submit_component().
 */
function _webform_submit_matrix($component, $value) {
   
    return serialize($value);
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_matrix($component, $value, $format = 'html') {

	
    $value = isset($value[0]) ? unserialize($value[0]) : '';
    //$value=  unserialize($value);
    //$value = webform_matrix_array(isset($value[0]) ? $value[0] : '', 'matrix');
    return array(
        '#title' => $component['name'],
        '#weight' => $component['weight'],
        '#theme' => 'webform_display_matrix',
        '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
        '#format' => $format,
        '#value' => $value,
        '#translatable' => array('title'),
    );
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_matrix($variables) {

    $element = $variables['element'];
	$sub_elements=$element['#webform_component']['extra']['element'];
	$headers=array();
	$value_array=$element['#value'];
	$col=1;
	foreach($sub_elements as $sub_element){
		$headers[]=$sub_element['title'];
		if($sub_element['type']=='select'){
			$option_array=matrix_select_option_from_text($sub_element['option']);
			foreach($value_array as $key=> $value_row){
				$value_array[$key][$col]=array_key_exists($value_row[$col],$option_array)?$option_array[$value_row[$col]]:$value_row[$col];
			}
		}
		$col++;
	}
	$output=theme('table',array('header'=>$headers,'rows'=>$value_array));
    return $output;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_matrix($component, $sids = array()) {

    $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
            ->fields('wsd', array('no', 'data'))
            ->condition('nid', $component['nid'])
            ->condition('cid', $component['cid'])
            ->orderBy('sid');

    if (count($sids)) {
        $query->condition('sid', $sids, 'IN');
    }

    $result = $query->execute();

    $matrixs = array();
    $submissions = 0;
    foreach ($result as $row) {
        $submissions++;
        if ($row['data']) {
            $matrixs[] = webform_matrix_array($row['data']);
        }
    }

    // Display stats.
    $nonblanks = count($matrixs);
    $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
    $rows[1] = array(t('User entered value'), $nonblanks);
    return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_matrix($component, $value) {

    if ($value[0]) {

		$sub_elements=$component['extra']['element'];

		$value_array=unserialize($value[0]);
		$col=1;
		$headers=array();
		foreach($sub_elements as $sub_element){
			$headers[]=$sub_element['title'];
			if($sub_element['type']=='select'){
				$option_array=matrix_select_option_from_text($sub_element['option']);
				foreach($value_array as $key=> $value_row){
					if(isset($value_row[$col]))
						$value_array[$key][$col]=array_key_exists($value_row[$col],$option_array)?$option_array[$value_row[$col]]:$value_row[$col];
				}
			}
			$col++;
		}
		$output=theme('table',array('header'=>$headers,'rows'=>$value_array));
	    return $output;
    } else {
        return '';
    }
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_matrix($component, $export_options) {
    $header = array();
    $header[0] = '';
    $header[1] = $component['name'];
	$sub_elements=$component['extra']['element'];
	$headers=array();
	foreach($sub_elements as $sub_element){
			$headers[]=$sub_element['title'];
	}
    $header[2] = $headers;
    return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_matrix($component, $export_options, $value) {
	
    if ($value[0]) {
		$value_array=webform_matrix_array($value[0]);
		$sub_elements=$component['extra']['element'];
		$col=1;
		foreach($sub_elements as $sub_element){
			if($sub_element['type']=='select'){
				$option_array=matrix_select_option_from_text($sub_element['option']);
				foreach($value_array as $key=> $value_row){
					if(isset($value_row[$col]))
						$value_array[$key][$col]=array_key_exists($value_row[$col],$option_array)?$option_array[$value_row[$col]]:$value_row[$col];
				}
			}
			$col++;
		}
		
		$reverse_array=matrix_array_reverse($value_array);

		$csv_array=array();
		foreach($reverse_array as $value){
			$csv_array[]=implode('-',$value);
		}
		return $csv_array;
    } else {
        return '';
    }
}

function webform_matrix_array($value) {

    return ($value != '') ? unserialize($value) : "";
}


function matrix_select_option_from_text($options){
	if($options!=''){
		foreach (preg_split("/((\r?\n)|(\r\n?))/", $options) as $line) {
			list($key, $value) = explode('|', $line);
			$option_array[$key] = $value;
		}
	}
	$option_array=(count($option_array)==0)?array(0=>'None'):$option_array;
	return $option_array;
}

function matrix_array_reverse($array){
	$rows=count($array);
	$cols=count($array[0]);
	$ridx=0;
	$cidx=0;
	$out=array();
	foreach($array as $row){
		foreach($row as  $val){
			$out[$ridx][$cidx]=$val;
			$ridx++;
			if($ridx>=$rows){
				$cidx++;
				$ridx=0;
			}
		}
	}
	return $out;
}