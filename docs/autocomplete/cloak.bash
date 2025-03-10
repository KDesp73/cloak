_cloak_cmd_0 () {
    compgen -A file "$1"
}

_cloak () {
    if [[ $(type -t _get_comp_words_by_ref) != function ]]; then
        echo _get_comp_words_by_ref: function not defined.  Make sure the bash-completions system package is installed
        return 1
    fi

    local words cword
    _get_comp_words_by_ref -n "$COMP_WORDBREAKS" words cword

    declare -a literals=(decrypt rsa verify -i files --gitignore-ignore init ls ini encrypt hash --type --cloakignore-ignore --output -o generate autocomplete sign --help -v --version signatures --key --input)
    declare -A literal_transitions=()
    literal_transitions[0]="([0]=6 [2]=1 [17]=1 [19]=1 [6]=1 [7]=2 [18]=1 [9]=3 [10]=4 [12]=1 [15]=5 [20]=1)"
    literal_transitions[2]="([5]=20 [3]=15 [12]=19 [11]=13 [23]=15)"
    literal_transitions[3]="([3]=12 [23]=12)"
    literal_transitions[4]="([3]=9 [23]=9)"
    literal_transitions[5]="([11]=14)"
    literal_transitions[6]="([3]=17 [23]=17)"
    literal_transitions[7]="([13]=8 [14]=8 [22]=9)"
    literal_transitions[10]="([13]=11 [5]=1 [14]=11)"
    literal_transitions[13]="([4]=1 [21]=1)"
    literal_transitions[14]="([1]=1 [8]=1 [16]=1)"
    literal_transitions[16]="([11]=13 [5]=20 [12]=19)"
    literal_transitions[18]="([22]=9)"
    literal_transitions[19]="([11]=13)"
    literal_transitions[20]="([11]=13 [12]=19)"
    literal_transitions[21]="([5]=1)"
    declare -A match_anything_transitions=([15]=16 [9]=1 [17]=7 [8]=18 [12]=10 [11]=21)
    declare -A subword_transitions

    local state=0
    local word_index=1
    while [[ $word_index -lt $cword ]]; do
        local word=${words[$word_index]}

        if [[ -v "literal_transitions[$state]" ]]; then
            declare -A state_transitions
            eval "state_transitions=${literal_transitions[$state]}"

            local word_matched=0
            for literal_id in $(seq 0 $((${#literals[@]} - 1))); do
                if [[ ${literals[$literal_id]} = "$word" ]]; then
                    if [[ -v "state_transitions[$literal_id]" ]]; then
                        state=${state_transitions[$literal_id]}
                        word_index=$((word_index + 1))
                        word_matched=1
                        break
                    fi
                fi
            done
            if [[ $word_matched -ne 0 ]]; then
                continue
            fi
        fi

        if [[ -v "match_anything_transitions[$state]" ]]; then
            state=${match_anything_transitions[$state]}
            word_index=$((word_index + 1))
            continue
        fi

        return 1
    done

    declare -A literal_transitions_level_0=([0]="0 2 17 19 6 7 18 9 10 12 15" [19]="11" [10]="5 14" [6]="3" [5]="11" [20]="11 12" [3]="3" [2]="5 3 12 11" [14]="1 8 16" [13]="4 21" [7]="14 22" [21]="5" [16]="11 5 12" [4]="3" [18]="22")
    declare -A literal_transitions_level_1=([6]="23" [4]="23" [3]="23" [2]="23" [0]="20" [7]="13" [10]="13")
    declare -A subword_transitions_level_0=()
    declare -A subword_transitions_level_1=()
    declare -A commands_level_0=()
    declare -A commands_level_1=()
    declare -A specialized_commands_level_0=([15]="0" [17]="0" [9]="0" [12]="0" [11]="0" [8]="0")
    declare -A specialized_commands_level_1=()

    local -a matches=()
    local max_fallback_level=1
    local prefix="${words[$cword]}"
    for (( fallback_level=0; fallback_level <= max_fallback_level; fallback_level++ )) {
       eval "declare literal_transitions_name=literal_transitions_level_${fallback_level}"
       eval "declare -a transitions=(\${$literal_transitions_name[$state]})"
       for literal_id in "${transitions[@]}"; do
           local literal="${literals[$literal_id]}"
           if [[ $literal = "${prefix}"* ]]; then
               matches+=("$literal ")
           fi
       done

       eval "declare subword_transitions_name=subword_transitions_level_${fallback_level}"
       eval "declare -a transitions=(\${$subword_transitions_name[$state]})"
       for subword_id in "${transitions[@]}"; do
           readarray -t -O "${#matches[@]}" matches < <(_cloak_subword_$subword_id complete "$prefix")
       done

       eval "declare commands_name=commands_level_${fallback_level}"
       eval "declare -a transitions=(\${$commands_name[$state]})"
       for command_id in "${transitions[@]}"; do
           local completions=()
           readarray -t completions < <(_cloak_cmd_$command_id "$prefix" | cut -f1)
           for item in "${completions[@]}"; do
               if [[ $item = "${prefix}"* ]]; then
                   matches+=("$item")
               fi
           done
       done

       eval "declare specialized_commands_name=specialized_commands_level_${fallback_level}"
       eval "declare -a transitions=(\${$specialized_commands_name[$state]})"
       for command_id in "${transitions[@]}"; do
           local completions=()
           readarray -t completions < <(_cloak_cmd_"${command_id}" "$prefix" | cut -f1)
           for item in "${completions[@]}"; do
               if [[ $item = "${prefix}"* ]]; then
                   matches+=("$item")
               fi
           done
       done

       if [[ ${#matches[@]} -gt 0 ]]; then
           local shortest_suffix="$prefix"
           for ((i=0; i < ${#COMP_WORDBREAKS}; i++)); do
               local char="${COMP_WORDBREAKS:$i:1}"
               local candidate=${prefix##*$char}
               if [[ ${#candidate} -lt ${#shortest_suffix} ]]; then
                   shortest_suffix=$candidate
               fi
           done
           local superfluous_prefix=""
           if [[ "$shortest_suffix" != "$prefix" ]]; then
               local superfluous_prefix=${prefix%$shortest_suffix}
           fi
           COMPREPLY=("${matches[@]#$superfluous_prefix}")
           break
       fi
    }

    return 0
}

complete -o nospace -F _cloak cloak
