function _cloak_spec_6
set 1 $argv[1]
__fish_complete_path "$1"
end

function _cloak
    set COMP_LINE (commandline --cut-at-cursor)

    set COMP_WORDS
    echo $COMP_LINE | read --tokenize --array COMP_WORDS
    if string match --quiet --regex '.*\s$' $COMP_LINE
        set COMP_CWORD (math (count $COMP_WORDS) + 1)
    else
        set COMP_CWORD (count $COMP_WORDS)
    end

    set literals "decrypt" "-i" "--gitignore-ignore" "init" "ls" "ini" "generate" "hash" "encrypt" "--cloakignore-ignore" "--output" "-o" "--type" "autocomplete" "--help" "-v" "--version" "--key" "--input"

    set descriptions
    set descriptions[1] "Decrypt a file or a directory"
    set descriptions[2] "Specify the input path"
    set descriptions[3] "Do not include the gitignore patterns"
    set descriptions[4] "Initialize cloak in your project"
    set descriptions[5] "Lists the files considered in the encryption process"
    set descriptions[7] "Generates various files"
    set descriptions[8] "Prints the hash of a file"
    set descriptions[9] "Encrypt a file or a directory"
    set descriptions[10] "Do not include the cloakignore patterns"
    set descriptions[12] "Specify the output path"
    set descriptions[13] "Specify the type"
    set descriptions[15] "Prints the help message"
    set descriptions[16] "Prints the project version"
    set descriptions[18] "Specify the encryption key path"

    set literal_transitions
    set literal_transitions[1] "set inputs 1 16 4 5 15 7 8 9 10 17; set tos 7 2 2 3 2 4 5 6 2 2"
    set literal_transitions[3] "set inputs 2 10 3 19; set tos 13 2 15 13"
    set literal_transitions[4] "set inputs 13; set tos 16"
    set literal_transitions[5] "set inputs 2 19; set tos 12 12"
    set literal_transitions[6] "set inputs 2 19; set tos 17 17"
    set literal_transitions[7] "set inputs 2 19; set tos 10 10"
    set literal_transitions[9] "set inputs 18; set tos 12"
    set literal_transitions[11] "set inputs 11 12 18; set tos 8 8 12"
    set literal_transitions[14] "set inputs 3 10; set tos 15 2"
    set literal_transitions[15] "set inputs 10; set tos 2"
    set literal_transitions[16] "set inputs 6 14; set tos 2 2"
    set literal_transitions[18] "set inputs 11 3 12; set tos 19 2 19"
    set literal_transitions[20] "set inputs 3; set tos 2"

    set match_anything_transitions_from 10 19 13 8 17 12
    set match_anything_transitions_to 11 20 14 9 18 2

    set state 1
    set word_index 2
    while test $word_index -lt $COMP_CWORD
        set -- word $COMP_WORDS[$word_index]

        if set --query literal_transitions[$state] && test -n $literal_transitions[$state]
            set --erase inputs
            set --erase tos
            eval $literal_transitions[$state]

            if contains -- $word $literals
                set literal_matched 0
                for literal_id in (seq 1 (count $literals))
                    if test $literals[$literal_id] = $word
                        set index (contains --index -- $literal_id $inputs)
                        set state $tos[$index]
                        set word_index (math $word_index + 1)
                        set literal_matched 1
                        break
                    end
                end
                if test $literal_matched -ne 0
                    continue
                end
            end
        end

        if set --query match_anything_transitions_from[$state] && test -n $match_anything_transitions_from[$state]
            set index (contains --index -- $state $match_anything_transitions_from)
            set state $match_anything_transitions_to[$index]
            set word_index (math $word_index + 1)
            continue
        end

        return 1
    end

    if set --query literal_transitions[$state] && test -n $literal_transitions[$state]
        set --erase inputs
        set --erase tos
        eval $literal_transitions[$state]
        for literal_id in $inputs
            if test -n $descriptions[$literal_id]
                printf '%s\t%s\n' $literals[$literal_id] $descriptions[$literal_id]
            else
                printf '%s\n' $literals[$literal_id]
            end
        end
    end

    set specialized_command_states 17 10 19 13 8 12
    set specialized_command_ids 6 6 6 6 6 6
    if contains $state $specialized_command_states
        set index (contains --index $state $specialized_command_states)
        set function_id $specialized_command_ids[$index]
        set function_name _cloak_spec_$function_id
        set --erase inputs
        set --erase tos
        set lines (eval $function_name $COMP_WORDS[$COMP_CWORD])
        for line in $lines
            printf '%s\n' $line
        end
    end

    return 0
end

complete --command cloak --no-files --arguments "(_cloak)"
