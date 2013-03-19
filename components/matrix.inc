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
  return
    array(
      'webform_matrix' => array(
        'render element' => 'element',
        'file' => 'components/matrix.inc',
        'path' => drupal_get_path('module', 'webform_matrix_component'),
      ),
      'webform_display_matrix' => array(
        'render element' => 'element',
        'file' => 'components/matrix.inc',
        'path' => drupal_get_path('module', 'webform_matrix_component'),
      ),
      'webform_date_matrix' => array(
        'render element' => 'element',
        'file' => 'components/matrix.inc',
        'path' => drupal_get_path('module', 'webform_matrix_component'),
      ),
    );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_matrix($component) {

  $form = array();
  $max_rows = variable_get('webform_matrix_rows', 10);
  $form['extra']['matrix_row'] = array(
    '#type' => 'select',
    '#title' => t('Matrix Rows'),
    '#options' => array_combine(range(1, $max_rows), range(1, $max_rows)),
    '#default_value' => isset($component['extra']['matrix_row']) ? $component['extra']['matrix_row'] : "1",
    '#weight' => 0,
    '#parents' => array('extra', 'matrix_row'),
  );

  $form['extra']['add_more'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Add More Button'),
    '#default_value' => isset($component['extra']['add_more']) ? $component['extra']['add_more'] : 0,
    '#weight' => 0,
    '#parents' => array('extra', 'add_more'),
    '#return_value' => 1,
  );

  return $form;
}

/**
 * Webform_edit_form extra form elements.
 * @see webform_matrix_component_form_webform_component_edit_form_alter()
 */
function webform_matrix_get_column_form($form, $form_state, $component) {

  $max_cols = variable_get('webform_matrix_cols', 10);
  $form['#attached']['css'][] = drupal_get_path('module', 'webform_matrix_component') . '/webform_matrix_component.css';
  $form['extra']['matrix_col'] = array(
    '#type' => 'select',
    '#title' => t('Matrix Column'),
    '#options' => array_combine(range(1, $max_cols), range(1, $max_cols)),
    '#default_value' => isset($component['extra']['matrix_col']) ? $component['extra']['matrix_col'] : "",
    '#description' => t('Choose this will remove all your previos settings'),
    '#parents' => array('extra', 'matrix_col'),
    '#ajax' => array(
      'wrapper' => 'matrix-settings-ajax',
      'callback' => '_matrix_get_type_option',
    ),
  );

  $form['extra']['element'] = array(
    '#type' => 'item',
    '#title' => t('Configuration'),
    '#prefix' => '<div id="matrix-settings-ajax">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  $input_component = !empty($form_state['values']) ? $form_state['values']['extra'] : $component['extra'];
  $matrix_col = isset($input_component['matrix_col']) ? $input_component['matrix_col'] : 1;
  $ele_options = array(
    'textfield' => 'Textfield',
    'select' => 'Select',
    'label' => 'Label',
    'date' => 'Date',
  );

  for ($i = 1; $i <= $matrix_col; $i++) {
    $element_type = 'textfield';
    $element_id = $element_title = 'element-' . $i;
    $option_array = $label_name = "";
    $element_mandatory = $is_datepicker = 0;
    if (isset($input_component['element'][$element_id])) {
      $elementvalue = $input_component['element'][$element_id];
      $element_type = @$elementvalue['type'];
      $element_title = @$elementvalue['title'];
      $element_mandatory = @$elementvalue['mandatory'];
      $is_datepicker = @$elementvalue['datepicker'];
    }

    $td = "<td id='$element_id'>";
    $prefix = ($i == 1) ? "<table id='matrix-options'><tbody><tr>" . $td : $td;
    $suffix = ($matrix_col == $i) ? "</td></tr></tbody></table>" : "</td>";

    $form['extra']['element'][$element_id]['#prefix'] = $prefix;
    $form['extra']['element'][$element_id]['#suffix'] = $suffix;

    $form['extra']['element'][$element_id]['type'] = array(
      '#type' => 'select',
      '#title' => $element_id,
      '#options' => $ele_options,
      '#default_value' => $element_type,
      '#parents' => array('extra', 'element', $element_id, 'type'),
      '#id' => "edit-extra-element-$element_id-type",
      '#ajax' => array(
        'wrapper' => 'matrix-settings-ajax',
        'callback' => '_matrix_get_type_option',
      ),
    );

    $form['extra']['element'][$element_id]['title'] = array(
      '#type' => 'textfield',
      '#title' => 'Title',
      '#size' => 15,
      '#default_value' => $element_title,
      '#parents' => array('extra', 'element', $element_id, 'title'),
      '#id' => "edit-extra-element-$element_id-title",
      '#required' => TRUE,
    );

    $form['extra']['element'][$element_id]['mandatory'] = array(
      '#type' => 'checkbox',
      '#title' => 'Mandatory',
      '#size' => 15,
      '#default_value' => $element_mandatory,
      '#parents' => array('extra', 'element', $element_id, 'mandatory'),
      '#return_value' => 1,
    );

    switch ($element_type) {

      case 'textfield':
        $validate_default = @$elementvalue['validate'];
        $validate_option_text = variable_get('textfield_validation', 'Numeric');
        $form['extra']['element'][$element_id]['validate'] = array(
          '#type' => 'select',
          '#title' => 'Validate',
          '#default_value' => $validate_default,
          '#parents' => array('extra', 'element', $element_id, 'validate'),
          '#options' => matrix_select_option_from_text($validate_option_text),
          '#empty_key' => 'None',
          '#empty_value' => 'None',
        );
        break;

      case 'select':
        $option_array = @$elementvalue['option'];
        $form['extra']['element'][$element_id]['option'] = array(
          '#type' => 'textarea',
          '#title' => 'Option',
          '#rows' => 2,
          '#cols' => 15,
          '#default_value' => $option_array,
          '#parents' => array('extra', 'element', $element_id, 'option'),
          '#required' => TRUE,
          '#element_validate' => array('webform_edit_matrix_select'),
          '#description' => t('<strong>Key-value pairs MUST be specified as "safe_key|Some readable option"</strong>. Use of only alphanumeric characters and underscores is recommended in keys.'),
        );
        break;

      case 'label':
        $label_name = @$elementvalue['label_name'];
        $form['extra']['element'][$element_id]['label_name'] = array(
          '#type' => 'textfield',
          '#title' => 'Label Name',
          '#default_value' => $label_name,
          '#size' => 15,
          '#required' => TRUE,
          '#parents' => array('extra', 'element', $element_id, 'label_name'),
        );
        break;

      case 'date':
        $form['extra']['element'][$element_id]['datepicker'] = array(
          '#type' => 'checkbox',
          '#title' => 'Calender',
          '#default_value' => $is_datepicker,
          '#parents' => array('extra', 'element', $element_id, 'datepicker'),
          '#return_value' => 1,
          '#description' => t('Default date type or calender one type element will show only.'),
        );

        $start_date = @$elementvalue['startdate'];
        $form['extra']['element'][$element_id]['startdate'] = array(
          '#type' => 'textfield',
          '#title' => 'Start Date',
          '#default_value' => !empty($start_date) ? $start_date : "-2 years",
          '#size' => 15,
          '#parents' => array('extra', 'element', $element_id, 'startdate'),
          '#description' => t('The earliest date that may be entered into the field.') . '<br/>' . t('Accepts any date in any <a href="http://www.gnu.org/software/tar/manual/html_chapter/Date-input-formats.html">GNU Date Input Format</a>.'),
        );

        $end_date = @$elementvalue['enddate'];
        $form['extra']['element'][$element_id]['enddate'] = array(
          '#type' => 'textfield',
          '#title' => 'End Date',
          '#default_value' => !empty($end_date) ? $end_date : "+2 years",
          '#size' => 15,
          '#parents' => array('extra', 'element', $element_id, 'enddate'),
          '#description' => t('The latest date that may be entered into the field.') . '<br/>' . t('Accepts any date in any <a href="http://www.gnu.org/software/tar/manual/html_chapter/Date-input-formats.html">GNU Date Input Format</a>.'),
        );

        $default_date = @$elementvalue['default_date'];
        $form['extra']['element'][$element_id]['default_date'] = array(
          '#type' => 'textfield',
          '#title' => 'Default Date',
          '#default_value' => !empty($default_date) ? $default_date : '',
          '#size' => 15,
          '#parents' => array('extra', 'element', $element_id, 'default_date'),
        );
        break;
    }
    drupal_alter('webform_edit_matrix', $form['extra']['element'][$element_id], $elementvalue, $element_id);
  }

  return $form;
}

/**
 * @see webform_matrix_get_column_form()
 */
function _matrix_get_type_option(&$form, &$form_state) {
  return $form['extra']['element'];
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_matrix($component, $value = NULL, $filter = TRUE) {

  module_load_include('inc', 'webform', '/components/date');
  $nid = $component['nid'];
  $cid = $component['cid'];
  $formkey = $component['form_key'];
  $pid = $component['pid'];
  $element = array();
  $element['#webform_component'] = $component;
  $element['#attached']['css'][] = drupal_get_path('module', 'webform_matrix_component') . '/webform_matrix_component.css';
  $element['value'] = array(
    '#markup' => "<label>" . $component['name'] . "</label>",
  );
  $datepicker = FALSE;

  if (isset($component['extra']['matrix_col'])) {
    $editvalue = unserialize($value[0]);
    $matrix_col = $component['extra']['matrix_col'];
    $matrix_row = $component['extra']['matrix_row'];
    for ($prow = 1; $prow <= $matrix_row; $prow++) {
      if (isset($component['extra']['element'])) {

        $element[$prow]['#prefix'] = "<div class='matrix-render-row-div matrix-render-col-$matrix_col'>";
        $element[$prow]['#suffix'] = "</div>";

        foreach ($component['extra']['element'] as $elekey => $elevalue) {
          list($j, $i) = explode('-', $elekey);

          $width = 100 / $matrix_col;
          $width = $width - .001;
          $prefix = "<div style='width:$width% !important;' class='matrix-row-column-element'>";
          $suffix = "</div>";
          $eletype = $elevalue['type'];
          $title = isset($elevalue['title']) ? $elevalue['title'] : $elekey;
          $title_display = ($prow == 1) ? 'before' : "invisible";

          // Only first row is manadatory.
          $required = FALSE;
          if ($prow == 1) {
            // If component is mandatory.
            $required = $component['mandatory'] ? TRUE : FALSE;

            if (!$required) {
              // If particular element is manadatory.
              $required = $elevalue['mandatory'] ? TRUE : FALSE;
            }
          }

          switch ($eletype) {

            case 'textfield':
              $element[$prow][$i] = array(
                '#type' => 'textfield',
                '#title' => $title,
                '#prefix' => $prefix,
                '#suffix' => $suffix,
                '#maxlength' => 50,
                '#default_value' => $editvalue[$prow][$i],
                '#required' => $required,
                '#title_display' => $title_display,
              );
              if (isset($elevalue['validate']) && $elevalue['validate'] != 'None') {
                $element[$prow][$i]['#element_validate'] = array('webform_render_matrix_textfield_' . $elevalue['validate']);
              }

              break;

            case 'select':
              $options = $elevalue['option'];
              $option_array = array();
              $option_array = matrix_select_option_from_text($options);
              $element[$prow][$i] = array(
                '#type' => 'select',
                '#title' => $title,
                '#options' => $option_array,
                '#prefix' => $prefix,
                '#suffix' => $suffix,
                '#default_value' => $editvalue[$prow][$i],
                '#required' => $required,
                '#title_display' => $title_display,
                '#empty_option' => 'Select',
                '#empty_value' => '_none',
              );
              break;

            case 'date':
              $datepicker = isset($elevalue['datepicker']) ? $elevalue['datepicker'] : FALSE;
              $element[$prow][$i] = array(
                '#type' => 'date',
                '#title' => $title,
                '#title_display' => $title_display,
                '#required' => $required,
                '#start_date' => !empty($elevalue['startdate']) ? $elevalue['startdate'] : '-2 years',
                '#end_date' => !empty($elevalue['enddate']) ? $elevalue['enddate'] : '+2 years',
                '#year_textfield' => 0,
                '#default_value' => !empty($editvalue[$prow][$i]) ? $editvalue[$prow][$i] : $elevalue['default_date'],
                '#timezone' => 'user',
                '#process' => array('webform_expand_date'),
                '#theme' => 'webform_date_matrix',
                '#theme_wrappers' => array('webform_element'),
                '#element_validate' => array('webform_validate_date'),
                '#datepicker' => $datepicker,
                '#prefix' => $prefix,
                '#suffix' => $suffix,
              );
              break;

            case 'label':
              $label_name = $elevalue['label_name'];
              $element[$prow][$i] = array(
                '#type' => 'item',
                '#title' => $title,
                '#markup' => $label_name,
                '#attributes' => array('class' => array('form-item')),
                '#prefix' => $prefix,
                '#suffix' => $suffix,
                '#default_value' => $editvalue[$prow][$i],
                '#title_display' => $title_display,
              );
              break;
          }
          drupal_alter('webform_render_matrix', $component, $elevalue, $element[$prow][$i]);
        }
      }
    }
  }

  if ($datepicker) {
    $element['#attached']['library'] = array(array('system', 'ui.datepicker'));
  }

  $element['#theme'][] = 'webform_matrix';

  $element_validate_array = array();
  foreach (module_implements('webform_matrix_validate') as $module) {
    $element_validate_array[] = $module . '_webform_matrix_validate';
  }
  if (!empty($element_validate_array)) {
    $element['#element_validate'] = $element_validate_array;
  }

  return $element;
}

/**
 * Theme for webform matrix.
 */
function theme_webform_matrix($variables) {
  $element = $variables['element'];
  $component = $element['#webform_component'];
  $form_key = $component['form_key'];
  $pid = $component['pid'];
  $id = "edit-$form_key-$pid";
  $output = "<div class='matrix-render-div' id='$id'>";
  $output .= drupal_render_children($element);
  $output .= "</div>";
  return $output;
}

/**
 * Theme for date subcomponent for matrix.
 */
function theme_webform_date_matrix($variables) {

  drupal_add_js(drupal_get_path('module', 'webform') . '/js/webform.js');

  $element = $variables['element'];

  $element['year']['#attributes']['class'] = array('year');
  $element['month']['#attributes']['class'] = array('month');
  $element['day']['#attributes']['class'] = array('day');

  // Add error classes to all items within the element.
  if (form_get_error($element)) {
    $element['year']['#attributes']['class'][] = 'error';
    $element['month']['#attributes']['class'][] = 'error';
    $element['day']['#attributes']['class'][] = 'error';
  }

  $class = array('webform-container-inline');

  // Add the JavaScript calendar if available (provided by Date module package).
  if (!empty($element['#datepicker'])) {
    $class[] = 'webform-datepicker';
    $calendar_class = array('webform-calendar');
    if ($element['#start_date']) {
      $calendar_class[] = 'webform-calendar-start-' . $element['#start_date'];
    }
    if ($element['#end_date']) {
      $calendar_class[] = 'webform-calendar-end-' . $element['#end_date'];
    }
    $calendar_class[] = 'webform-calendar-day-' . variable_get('date_first_day', 0);

    $calendar = theme('webform_calendar', array('component' => NULL, 'calendar_classes' => $calendar_class));
  }

  $output = '';
  $output .= '<div class="' . implode(' ', $class) . '">';
  $output .= drupal_render_children($element);
  $output .= isset($calendar) ? $calendar : '';
  $output .= '</div>';

  return $output;
}

/**
 * Implements _webform_submit_component().
 */
function _webform_submit_matrix($component, $value) {

  return serialize($value);
}

/**
 * Validate webform_edit_form matrix components.
 */
function webform_edit_matrix_select($element, &$form_state) {

  $lines = explode("\n", trim($element['#value']));
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
      // No need to store group names.
      $key = NULL;
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
    form_error($element, t('Every option must have a key specified. Specify each option as "safe_key|Some readable option".'));
  }

  if (!empty($long_keys)) {
    form_error($element, t('Option keys must be less than 128 characters. The following keys exceed this limit:') . theme('item_list', $long_keys));
  }

  if (!empty($duplicate_keys)) {
    form_error($element, t('Options within the select list must be unique. The following keys have been used multiple times:') . theme('item_list', array('items' => $duplicate_keys)));
  }
}

/**
 * Check if the user filled the required fields.
 */
function webform_edit_matrix_date($element, $form_state) {
  // Check if the user filled the required fields.
  foreach (array('day', 'month', 'year') as $field_type) {
    if (!is_numeric($element[$field_type]['#value']) && $element['#required']) {
      form_error($element, t('!name field is required.', array('!name' => $element['#title'])));
      return;
    }
  }
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_matrix($component, $value, $format = 'html') {

  $headers = array();
  $value_array = isset($value[0]) ? unserialize($value[0]) : '';
  $sub_elements = $component['extra']['element'];
  $col = 1;
  foreach ($sub_elements as $sub_element) {
    $headers[] = $sub_element['title'];
    if ($sub_element['type'] == 'select') {
      $option_array = matrix_select_option_from_text($sub_element['option']);
      foreach ($value_array as $key => $value_row) {
        $value_array[$key][$col] = array_key_exists($value_row[$col], $option_array) ? $option_array[$value_row[$col]] : $value_row[$col];
      }
    } 
    elseif ($sub_element['type'] == 'date') {
      foreach ($value_array as $key => $value_row) {
        if ($value_array[$key][$col]['year'] && $value_array[$key][$col]['month'] && $value_array[$key][$col]['day']) {
          $timestamp = webform_strtotime($value_array[$key][$col]['month'] . '/' . $value_array[$key][$col]['day'] . '/' . $value_array[$key][$col]['year']);
          $format = webform_date_format('medium');
          $value_array[$key][$col] = format_date($timestamp, 'custom', $format, 'UTC');
        }
        else {
          $value_array[$key][$col] = "";
        }
      }
    }
    $col++;
  }
  $matrix_value = array('headers' => $headers, 'rows' => $value_array);

  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_matrix',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => $matrix_value,
    '#translatable' => array('title'),
  );
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_matrix($variables) {

  $matrix_value = $variables['element']['#value'];
  $output = theme('table', array('header' => $matrix_value['headers'], 'rows' => $matrix_value['rows']));
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

    $sub_elements = $component['extra']['element'];

    $value_array = unserialize($value[0]);
    $col = 1;
    $headers = array();
    foreach ($sub_elements as $sub_element) {
      $headers[] = $sub_element['title'];
      if ($sub_element['type'] == 'select') {
        $option_array = matrix_select_option_from_text($sub_element['option']);
        foreach ($value_array as $key => $value_row) {
          if (isset($value_row[$col]))
            $value_array[$key][$col] = array_key_exists($value_row[$col], $option_array) ? $option_array[$value_row[$col]] : $value_row[$col];
        }
      }
      elseif ($sub_element['type'] == 'date') {
        foreach ($value_array as $key => $value_row) {
          if ($value_array[$key][$col]['year'] && $value_array[$key][$col]['month'] && $value_array[$key][$col]['day']) {
            $timestamp = webform_strtotime($value_array[$key][$col]['month'] . '/' . $value_array[$key][$col]['day'] . '/' . $value_array[$key][$col]['year']);
            $format = webform_date_format('short');
            $value_array[$key][$col] = format_date($timestamp, 'custom', $format, 'UTC');
          }
          else {
            $value_array[$key][$col] = "";
          }
        }
      }
      $col++;
    }
    $output = theme('table', array('header' => $headers, 'rows' => $value_array));
    return $output;
  }
  else {
    return '';
  }
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_matrix($component, $export_options) {
  $header = array();
  $sub_elements = $component['extra']['element'];
  foreach ($sub_elements as $sub_element) {
    $header[1][] = $component['name'];
    $header[2][] = $sub_element['title'];
  }
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_matrix($component, $export_options, $value) {

  if ($value[0]) {
    $value_array = webform_matrix_array($value[0]);
    $sub_elements = $component['extra']['element'];
    $col = 1;
    foreach ($sub_elements as $sub_element) {
      if ($sub_element['type'] == 'select') {
        $option_array = matrix_select_option_from_text($sub_element['option']);
        foreach ($value_array as $key => $value_row) {
          if (isset($value_row[$col]))
            $value_array[$key][$col] = array_key_exists($value_row[$col], $option_array) ? $option_array[$value_row[$col]] : $value_row[$col];
        }
      }
      elseif ($sub_element['type'] == 'date') {
        foreach ($value_array as $key => $value_row) {
          if ($value_array[$key][$col]['year'] && $value_array[$key][$col]['month'] && $value_array[$key][$col]['day']) {
            $timestamp = webform_strtotime($value_array[$key][$col]['month'] . '/' . $value_array[$key][$col]['day'] . '/' . $value_array[$key][$col]['year']);
            $format = webform_date_format('short');
            $value_array[$key][$col] = format_date($timestamp, 'custom', $format, 'UTC');
          }
          else {
            $value_array[$key][$col] = "";
          }
        }
      }
      $col++;
    }

    $reverse_array = matrix_array_reverse($value_array);

    $csv_array = array();
    foreach ($reverse_array as $value) {
      $csv_array[] = implode(" - ", $value);
    }
    return $csv_array;
  }
  else {
    return '';
  }
}

/**
 * Unseriaze matrix components values.
 */
function webform_matrix_array($value) {
  return ($value != '') ? unserialize($value) : "";
}

/**
 * Convert matrix select option values to options array.
 */
function matrix_select_option_from_text($options) {
  if ($options != '') {
    foreach (preg_split("/((\r?\n)|(\r\n?))/", $options) as $line) {
      if (strstr($line, '|')) {
        list($key, $value) = explode('|', $line);
      }
      else {
        $key = $value = $line;
      }
      $option_array[$key] = $value;
    }
  }
  $option_array = (count($option_array) == 0) ? array(0 => 'None') : $option_array;
  return $option_array;
}

/**
 * This is simple function to convert array rows to cols.
 */
function matrix_array_reverse($array) {
  $out = array();
  foreach ($array as $rowkey => $row) {
    foreach ($row as $colkey => $col) {
      $out[$colkey][$rowkey] = $col;
    }
  }
  return $out;
}

/**
 * Validate textfield Numeric validation.
 */
function webform_render_matrix_textfield_Numeric($element, $form_state) {
  $value = $element['#value'];
  if (!empty($value)) {
    if (!is_numeric($value)) {
      form_error($element, t('Only Numeric value is allowed'));
    }
  }
}

